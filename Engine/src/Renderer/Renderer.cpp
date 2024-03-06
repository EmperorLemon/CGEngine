#include "Renderer.h"
#include "Context.h"

#include <span>

#include "Platform/OpenGL/OpenGL.h"

#include "RenderAPI.h"
#include "Shader.h"

#include "Component/Mesh.h"

#include "IO/FileSystem.h"

namespace CGEngine
{
	std::shared_ptr<OpenGL::GLBuffer> vertex_buffer = nullptr;
	std::shared_ptr<OpenGL::GLVertexArray> vertex_array = nullptr;
	std::shared_ptr<OpenGL::GLShader> shader = nullptr;

	GraphicsAPI Renderer::m_API = GraphicsAPI::CG_NO_API;

	Renderer::Renderer(const RendererCreateInfo& rendererInfo)
	{
		m_API = rendererInfo.API;
		m_window = std::make_shared<Window>(rendererInfo.window);

		DeviceProperties deviceProperties = {};
		const ContextCreateInfo context = {m_window, deviceProperties};

		CreateContext(context);
		m_backend = std::make_shared<OpenGL::OpenGLAPI>();
	}

	void Renderer::PreRender()
	{
		VertexLayout layout;
		{
			layout.add(0, 3, 0, 3 * sizeof(float))
			      .end();
		}

		Object::Mesh mesh = {};
		LoadModelFile("Assets/Models/triangle.gltf", IO::ModelFileType::glTF, mesh);

		const size_t vBufferSize = sizeof(float) * mesh.vertices.size();
		const size_t iBufferSize = sizeof(uint16_t) * mesh.indices.size();
		const size_t bufferSize = vBufferSize + iBufferSize;

		const BufferInfo vBufferInfo = { vBufferSize, mesh.vertices.size(), 0 };
		const BufferInfo iBufferInfo = { iBufferSize, mesh.indices.size(), vBufferInfo.size };

		vertex_buffer = std::make_shared<OpenGL::GLBuffer>(bufferSize, nullptr);
		vertex_buffer->SetSubData(vBufferInfo.offset, vBufferInfo.size, mesh.vertices.data());
		vertex_buffer->SetSubData(iBufferInfo.offset, iBufferInfo.size, mesh.indices.data());

		vertex_array = std::make_shared<OpenGL::GLVertexArray>(vertex_buffer->GetID(), vBufferInfo, &iBufferInfo, layout);

		std::string vert_src, frag_src;
		IO::ReadFile("Assets/Shaders/unlit.vert", vert_src);
		IO::ReadFile("Assets/Shaders/unlit.frag", frag_src);

		ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
		shader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
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