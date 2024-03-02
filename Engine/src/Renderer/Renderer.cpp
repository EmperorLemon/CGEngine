#include "Renderer.h"
#include "Context.h"

#include "RenderAPI.h"
#include "Platform/OpenGL/OpenGLAPI.h"

namespace CGEngine
{
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
	}

	void Renderer::Render()
	{
		float RGBA[4] = { 0.2f, 0.45f, 0.6f, 1.0f };

		m_backend->Clear(static_cast<uint32_t>(ClearMask::CG_COLOR_BUFFER_BIT));
		m_backend->ClearColor(RGBA);

		SwapBuffers(m_window);
	}

	void Renderer::PostRender()
	{
	}

	GraphicsAPI Renderer::GetAPI()
	{
		return m_API;
	}
}