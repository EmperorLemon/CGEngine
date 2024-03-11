#pragma once

#include <cstdint>

namespace CGEngine
{
	constexpr uint32_t CreateRGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255)
	{
		return ((r & 0xFF) << 24) + ((g & 0xFF) << 16) + ((b & 0xFF) << 8) + (a & 0xFF);
	}

	enum class ClearMask : uint32_t
	{
		NULL_BUFFER_BIT,
		COLOR_BUFFER_BIT,
		DEPTH_BUFFER_BIT,
		COLOR_DEPTH_BUFFER_BIT,
		STENCIL_BUFFER_BIT
	};

	enum class APICapability : uint32_t
	{
		BLEND,
		CULL_FACE,
		DEPTH_TEST,
		STENCIL_TEST,
		FRAMEBUFFER_SRGB,
		LINE_SMOOTH,
		POLYGON_SMOOTH,
		SCISSOR_TEST,
	};

	class RenderAPI
	{
	public:
		RenderAPI() = default;

		RenderAPI(RenderAPI&&) noexcept = delete;
		RenderAPI(const RenderAPI&) noexcept = delete;
		RenderAPI& operator=(RenderAPI&&) noexcept = delete;
		RenderAPI& operator=(const RenderAPI&) noexcept = delete;

		virtual ~RenderAPI() = default;

		virtual void Draw(void* ptr) const = 0;

		virtual void Enable(uint32_t capability)  const = 0;
		virtual void Disable(uint32_t capability) const = 0;

		virtual void Clear(uint32_t mask) = 0;
		virtual void ClearColor(float* rgba) = 0;

		virtual void ResizeViewport(int32_t x, int32_t y, int32_t width, int32_t height) = 0;
	};
}
