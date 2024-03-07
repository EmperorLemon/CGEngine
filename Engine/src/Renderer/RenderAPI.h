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
		CG_NULL_BUFFER_BIT,
		CG_COLOR_BUFFER_BIT,
		CG_DEPTH_BUFFER_BIT,
		CG_COLOR_DEPTH_BUFFER_BIT,
		CG_STENCIL_BUFFER_BIT
	};

	enum class APICallHandle : uint32_t
	{
		CG_CLEAR_BUFFER,
		CG_CLEAR_COLOR,
	};

	enum class APICapability : uint32_t
	{
		CG_BLEND,
		CG_CULL_FACE,
		CG_DEPTH_TEST,
		CG_FRAMEBUFFER_SRGB,
		CG_LINE_SMOOTH,
		CG_POLYGON_SMOOTH,
		CG_SCISSOR_TEST,
		CG_STENCIL_TEST
	};

	class RenderAPI
	{
	public:
		RenderAPI() = default;

		RenderAPI(RenderAPI&&) noexcept = default;
		RenderAPI& operator=(RenderAPI&&) noexcept = default;
		RenderAPI(const RenderAPI&) = delete;
		RenderAPI& operator=(const RenderAPI&) = delete;

		virtual ~RenderAPI() = default;

		virtual void Draw(void* array_ptr) const = 0;

		virtual void Enable(uint32_t capability)  const = 0;
		virtual void Disable(uint32_t capability) const = 0;

		virtual void Clear(uint32_t mask) = 0;
		virtual void ClearColor(float* rgba) = 0;

		virtual void ResizeViewport(int32_t x, int32_t y, int32_t width, int32_t height) = 0;
	};
}
