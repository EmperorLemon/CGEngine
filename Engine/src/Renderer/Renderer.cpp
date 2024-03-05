#include "Renderer.h"
#include "Context.h"

#include <span>

#include "Platform/OpenGL/OpenGL.h"

#include "RenderAPI.h"
#include "Shader.h"

#include "IO/FileSystem.h"

namespace CGEngine
{
	std::shared_ptr<OpenGL::GLVertexArray> vertex_array = nullptr;
	std::shared_ptr<OpenGL::GLShader> shader = nullptr;

	float data_v[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	uint16_t data_i[] =
	{
		0, 1, 2
	};

	std::span vertices(data_v, std::size(data_v));
	std::span  indices(data_i, std::size(data_i));

	GraphicsAPI Renderer::m_API = GraphicsAPI::CG_NO_API;

	Renderer::Renderer(const RendererCreateInfo& rendererInfo)
	{
		m_API = rendererInfo.API;
		m_window = std::make_shared<Window>(rendererInfo.window);

		DeviceProperties deviceProperties = {};
		const ContextCreateInfo context = {m_window, deviceProperties};

		CreateContext(context);
		m_backend = std::make_shared<OpenGL::OpenGLAPI>();

		VertexLayout layout;
		{
			layout.add(0, 3, DataType::FLOAT)
			      .add(1, 3, DataType::FLOAT)
			      .end();
		}

		const auto vertex_buffer = std::make_shared<OpenGL::GLBuffer>(DataType::FLOAT,   vertices.size(), vertices.data());
		const auto index_buffer = std::make_shared<OpenGL::GLBuffer>(DataType::UNSIGNED_SHORT, indices.size(), indices.data());

		vertex_array = std::make_shared<OpenGL::GLVertexArray>(vertex_buffer.get(), index_buffer.get(), layout);

		std::string vert_src, frag_src;
		IO::ReadFile("Assets/Shaders/unlit.vert", vert_src);
		IO::ReadFile("Assets/Shaders/unlit.frag", frag_src);

		ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
		shader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
	}

	void Renderer::PreRender()
	{
	}

	void Renderer::Render()
	{
		float RGBA[4] = { 0.2f, 0.45f, 0.6f, 1.0f };

		m_backend->Clear(static_cast<uint32_t>(ClearMask::CG_COLOR_BUFFER_BIT));
		m_backend->ClearColor(RGBA);

		shader->Bind();
		vertex_array->Bind();
		m_backend->Draw(vertex_array.get());
		vertex_array->Unbind();

		SwapBuffers(m_window);
	}

	void Renderer::PostRender()
	{
		shader->Unbind();
	}

	GraphicsAPI Renderer::GetAPI()
	{
		return m_API;
	}
}