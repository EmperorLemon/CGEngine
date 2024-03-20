#pragma once

#include "./Core/Window.h"

#include <memory>

namespace CGEngine::Component
{
	struct DrawObject;
	struct Light;
	struct Transform;
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
		void RenderPrimitive(const Component::DrawObject& primitive) const;
		void PostRender() const;

		void Update(Camera& camera, const Time& time) const;
		void UpdateInstance(int32_t offset, const Component::Transform& transform) const;
		void UpdateLight(int32_t offset, const Component::Light& light) const;

		void FirstPass()  const;
		void SecondPass() const;
		void ThirdPass()  const;

		void ResizeProjection(int32_t width, int32_t height, Camera& camera) const;
		void ResizeViewport(int32_t width, int32_t height) const;
		void ResizeFramebuffer(int32_t width, int32_t height) const;

		[[nodiscard]] uint32_t GetColorTextureID() const;
		[[nodiscard]] uint32_t GetDepthTextureID() const;

		static GraphicsAPI GetAPI() { return m_API; }
	private:
		static GraphicsAPI m_API;

		Window& m_window;
		std::shared_ptr<RenderAPI> m_backend = nullptr;
	};
}
