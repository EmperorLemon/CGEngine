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

	enum class ShaderEnable : uint8_t
	{
		NONE = 0,
		DEFAULT_SHADER,
		DEPTH_SHADER,
		LIGHT_SHADER
	};

	enum class GraphicsAPI : uint8_t
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

		void EnableShader(ShaderEnable enable) const;

		void UpdateUniforms(Camera& camera, const Time& time) const;
		void UpdateInstance(int32_t offset, const Component::Transform& transform) const;

		void UpdateLight(int32_t offset, uint32_t count, const Component::Light& light) const;
		void DrawLight(const Component::Transform& transform, const Component::Light& light) const;

		void FirstPass()  const;
		void SecondPass() const;
		void ThirdPass()  const;

		void ResizeProjection(int32_t width, int32_t height, Camera& camera) const;
		void ResizeViewport(int32_t width, int32_t height) const;
		void ResizeFramebuffer(int32_t width, int32_t height) const;

		[[nodiscard]] uint32_t GetSceneTextureID() const;
		[[nodiscard]] uint32_t GetDepthTextureID() const;
		[[nodiscard]] uint32_t  GetGlowTextureID() const;

		static GraphicsAPI GetAPI() { return m_API; }
	private:
		static GraphicsAPI m_API;

		Window& m_window;
		std::shared_ptr<RenderAPI> m_backend = nullptr;
	};
}
