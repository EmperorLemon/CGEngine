#pragma once

#include "./Core/Window.h"

namespace CGEngine
{
	enum class RendererAPI
	{
		CG_NO_API,
		CG_OPENGL_API,
		CG_VULKAN_API,
		CG_D3D11_API,
		CG_D3D12_API
	};

	struct RendererCreateInfo
	{
		RendererAPI API = RendererAPI::CG_NO_API;
		Window window;
	};

	class Renderer
	{
	public:
		Renderer() = default;
		explicit Renderer(const RendererCreateInfo& rendererInfo);

		void PreRender();
		void Render();
		void PostRender();

		static RendererAPI GetAPI();
	private:
		static RendererAPI m_API;

		std::shared_ptr<Window> m_window = nullptr;
	};
}
