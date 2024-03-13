#pragma once

#include "./Core/Window.h"

#include <memory>

namespace CGEngine::Component
{
	struct Transform;
	struct DrawObject;
}

namespace CGEngine
{
	struct Time;
	struct Camera;
	class RenderAPI;

	enum class GraphicsAPI
	{
		CG_NO_API,
		CG_OPENGL_API,
		CG_VULKAN_API,
		CG_D3D11_API,
		CG_D3D12_API
	};

	struct RendererCreateInfo
	{
		GraphicsAPI API = GraphicsAPI::CG_NO_API;
		Window& window;
	};

	class Renderer
	{
	public:
		Renderer() = delete;
		explicit Renderer(const RendererCreateInfo& rendererInfo);

		void PreRender(Camera& camera);
		void Render(const Time& time);
		void RenderPrimitive(const Component::Transform& transform, const Component::DrawObject& primitive);
		void PostRender();

		void FirstPass()  const;
		void SecondPass() const;

		void ResizeProjection(Camera& camera) const;
		void ResizeViewport(int32_t width, int32_t height) const;
		void ResizeFramebuffer(int32_t width, int32_t height) const;

		static GraphicsAPI GetAPI();
	private:
		static GraphicsAPI m_API;

		Window& m_window;
		std::shared_ptr<RenderAPI> m_backend = nullptr;
	};
}
