#include "Renderer.h"
#include "Context.h"

namespace CGEngine
{
	RendererAPI Renderer::m_API = RendererAPI::CG_NO_API;

	Renderer::Renderer(const RendererCreateInfo& rendererInfo)
	{
		m_API = rendererInfo.API;
		m_window = std::make_shared<Window>(rendererInfo.window);

		DeviceProperties deviceProperties = {};
		const ContextCreateInfo context = {m_window, deviceProperties};

		CreateContext(context);
	}

	void Renderer::PreRender()
	{
		
	}

	void Renderer::Render()
	{

	}

	void Renderer::PostRender()
	{

	}

	RendererAPI Renderer::GetAPI()
	{
		return m_API;
	}
}