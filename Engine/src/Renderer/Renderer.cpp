#include "Renderer.h"

#include "Core/Logger.hpp"

#include "Context.h"
#include "Camera.h"

#include "Component/Mesh.h"
#include "IO/FileSystem.h"

#include "Math/Math.h"
#include "Math/Transform.h"
#include "Math/Vector3.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLDrawObject.h"

namespace CGEngine
{
	void SetupRenderScene();

	std::vector<std::shared_ptr<OpenGL::GLDrawObject>> objects;
	std::shared_ptr<OpenGL::GLShader> shader = nullptr;

	GraphicsAPI Renderer::m_API = GraphicsAPI::CG_NO_API;

	Renderer::Renderer(const RendererCreateInfo& rendererInfo) : m_window(rendererInfo.window)
	{
		DeviceProperties deviceProperties = {};
		const ContextCreateInfo context = { m_window, deviceProperties };

		CreateContext(context);
		m_backend = std::make_shared<OpenGL::OpenGLAPI>();

		SetupRenderScene();
	}

	void SetupRenderScene()
	{
		std::vector<Object::Mesh> meshes;
		LoadModelFile("Assets/Models/Test/test.gltf", IO::ModelFileType::glTF, meshes);

		if (!meshes.empty())
		{
			for (auto& mesh : meshes)
			{
				objects.emplace_back(std::make_shared<OpenGL::GLDrawObject>(std::move(mesh.vertices), std::move(mesh.indices), std::move(mesh.layout)));
			}
		}

		objects.at(0)->position = Math::Vector3(-1.5f, 0.0f, 0.0f);
		objects.at(1)->position = Math::Vector3(1.5f, 0.0f, 0.0f);

		std::string vert_src, frag_src;
		IO::ReadFile("Assets/Shaders/unlit.vert", vert_src);
		IO::ReadFile("Assets/Shaders/unlit.frag", frag_src);

		ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
		shader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
	}

	void Renderer::PreRender()
	{
		m_backend->Enable(static_cast<uint32_t>(APICapability::CG_DEPTH_TEST));

		constexpr auto camera_position = Math::Vector3(0.0f, 0.0f, 5.0f);
		m_camera = std::make_shared<Camera>(camera_position);

		m_camera->fov = GetFieldOfView(65.0f);
		m_camera->aspect = GetAspectRatio(m_window.width, m_window.height);

		m_camera->projection = Math::Perspective(m_camera->fov, m_camera->aspect, m_camera->near, m_camera->far);
		m_camera->view = Math::View(m_camera->position, m_camera->direction, m_camera->up);

		shader->Use();

		shader->BindUniform("uProjection", OpenGL::UniformType::MAT4, Math::value_ptr(m_camera->projection));
		shader->BindUniform("uView", OpenGL::UniformType::MAT4, Math::value_ptr(m_camera->view));
		shader->BindUniform("uModel", OpenGL::UniformType::MAT4, Math::value_ptr(Math::Mat4(1.0f)));
	}

	void Renderer::Render()
	{
		float RGBA[4] = { 0.2f, 0.45f, 0.6f, 1.0f };

		m_backend->Clear(static_cast<uint32_t>(ClearMask::CG_COLOR_DEPTH_BUFFER_BIT));
		m_backend->ClearColor(RGBA);

		shader->Use();
		for (const auto& object : objects)
		{
			auto model = Math::Mat4(1.0f);
			model = Math::Translate(model, object->position);

			shader->BindUniform("uModel", OpenGL::UniformType::MAT4, Math::value_ptr(model));
			m_backend->Draw(object.get());
		}

		SwapBuffers(m_window);
	}

	void Renderer::PostRender()
	{
		shader->Disable();
	}

	GraphicsAPI Renderer::GetAPI()
	{
		return m_API;
	}
}
