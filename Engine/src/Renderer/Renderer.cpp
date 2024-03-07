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
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace CGEngine
{
	void SetupRenderScene();

	std::shared_ptr<OpenGL::GLBuffer> vertex_buffer = nullptr;
	std::shared_ptr<OpenGL::GLVertexArray> vertex_array = nullptr;
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
		LoadModelFile("Assets/Models/cube.gltf", IO::ModelFileType::glTF, meshes);

		CG_TRACE("Vertices size: {0}", meshes.at(0).vertices.size());

		if (!meshes.empty())
		{
			meshes.at(0).layout.add(0, 3, DataType::FLOAT).add(1, 3, DataType::FLOAT).end();// .add(1, 3, DataType::FLOAT).end();

			const size_t vBufferSize = sizeof(float) * meshes.at(0).vertices.size();
			const size_t iBufferSize = sizeof(uint16_t) * meshes.at(0).indices.size();
			const size_t bufferSize = vBufferSize + iBufferSize;

			const BufferInfo vBufferInfo = { vBufferSize, meshes.at(0).vertices.size(), 0 };
			const BufferInfo iBufferInfo = { iBufferSize, meshes.at(0).indices.size(), vBufferInfo.size };

			vertex_buffer = std::make_shared<OpenGL::GLBuffer>(bufferSize, nullptr);
			vertex_buffer->SetSubData(vBufferInfo.offset, vBufferInfo.size, meshes.at(0).vertices.data());
			vertex_buffer->SetSubData(iBufferInfo.offset, iBufferInfo.size, meshes.at(0).indices.data());

			vertex_array = std::make_shared<OpenGL::GLVertexArray>(vertex_buffer->GetID(), vBufferInfo, &iBufferInfo, meshes.at(0).layout);
		}

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

		m_camera->fov = Math::DegToRad(45.0f);

		const int32_t height = m_window.height != 0 ? m_window.height : 1;
		m_camera->aspect = static_cast<float>(m_window.width) / static_cast<float>(height);

		m_camera->projection = Math::Perspective(m_camera->fov, m_camera->aspect, m_camera->near, m_camera->far);
		m_camera->view = Math::View(m_camera->position, m_camera->direction, m_camera->up);

		shader->Use();

		auto model = Math::Mat4(1.0f);

		model = Math::Rotate(model, 45.0f, Math::Y_AXIS);

		shader->BindUniform("uProjection", OpenGL::UniformType::MAT4, Math::value_ptr(m_camera->projection));
		shader->BindUniform("uView", OpenGL::UniformType::MAT4, Math::value_ptr(m_camera->view));
		shader->BindUniform("uModel", OpenGL::UniformType::MAT4, Math::value_ptr(model));

		shader->Disable();
	}

	void Renderer::Render()
	{
		float RGBA[4] = { 0.2f, 0.45f, 0.6f, 1.0f };

		m_backend->Clear(static_cast<uint32_t>(ClearMask::CG_COLOR_BUFFER_BIT));
		m_backend->ClearColor(RGBA);

		shader->Use();

		if (vertex_array != nullptr)
		{
			vertex_array->Bind();
			   m_backend->Draw(vertex_array.get());
			vertex_array->Unbind();
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
