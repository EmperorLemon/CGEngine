#include "Renderer.h"

#include "Core/Logger.hpp"

#include "Context.h"
#include "Camera.h"

#include "Renderer/Assets/Mesh.h"
#include "IO/FileSystem.h"

#include "Math/Math.h"
#include "Math/Transform.h"
#include "Math/Vector3.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLDrawObject.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace CGEngine
{
	void SetupRenderScene();

	std::vector<std::shared_ptr<OpenGL::GLDrawObject>> objects;

	std::shared_ptr<OpenGL::GLShader> shader = nullptr;
	std::shared_ptr<OpenGL::GLBuffer> uniformBuffer = nullptr;

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
		std::vector<Assets::Mesh> meshes;
		LoadModelFile("Assets/Models/Test/cube.gltf", IO::ModelFileType::glTF, meshes);

		if (!meshes.empty())
		{
			for (auto& mesh : meshes)
			{
				objects.emplace_back(std::make_shared<OpenGL::GLDrawObject>(std::move(mesh)));
			}
		}

		objects.at(0)->position = Math::Vector3(0.0f, 0.0f, 4.0f);
		objects.at(0)->scale = Math::Vector3(0.0f, 0.0f, 4.0f);

		std::string vert_src, frag_src;
		IO::ReadFile("Assets/Shaders/unlit.vert", vert_src);
		IO::ReadFile("Assets/Shaders/unlit.frag", frag_src);

		ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
		shader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));

		uniformBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::UNIFORM_BUFFER, 2 * sizeof(Math::Mat4), nullptr);
		uniformBuffer->BindBufferRange(1, 0, 2 * sizeof(Math::Mat4));
	}

	void Renderer::PreRender(const Camera& camera)
	{
		m_backend->Enable(static_cast<uint32_t>(APICapability::CG_DEPTH_TEST));

		uniformBuffer->SetSubData(0, sizeof(Math::Mat4), Math::value_ptr(camera.projection));
		uniformBuffer->SetSubData(sizeof(Math::Mat4), sizeof(Math::Mat4), Math::value_ptr(camera.projection));
	}

	void Renderer::Render()
	{
		float RGBA[4] = { 0.2f, 0.45f, 0.55f, 1.0f };

		m_backend->Clear(static_cast<uint32_t>(ClearMask::CG_COLOR_DEPTH_BUFFER_BIT));
		m_backend->ClearColor(RGBA);

		constexpr int base_color_texture_sampler = 0;
		constexpr int metallic_roughness_texture_sampler = 1;

		shader->Use();
		for (const auto& object : objects)
		{
			auto model = Math::Mat4(1.0f);

			model = Math::Translate(model, object->position);

			model = Math::Rotate(model, 0.0f, Math::X_AXIS);
			model = Math::Rotate(model, 45.0f, Math::Y_AXIS);
			model = Math::Rotate(model, 0.0f, Math::Z_AXIS);

			int unit = 0;

			for (const auto& texture : object->GetTextures())
			{
				texture->Bind(unit);
				unit++;
			}

			shader->BindUniform("uModel", OpenGL::UniformType::MAT4, Math::value_ptr(model));

			const auto& material = object->GetMaterial();

			shader->BindUniform("material.baseColor", OpenGL::UniformType::VEC4, Math::value_ptr(material.baseColor));
			shader->BindUniform("material.baseColorSampler", OpenGL::UniformType::INT, &base_color_texture_sampler);
			shader->BindUniform("material.metallicRoughnessSampler", OpenGL::UniformType::INT, &metallic_roughness_texture_sampler);
			shader->BindUniform("material.metallicFactor", OpenGL::UniformType::FLOAT, &material.metallicFactor);
			shader->BindUniform("material.roughnessFactor", OpenGL::UniformType::FLOAT, &material.roughnessFactor);

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
