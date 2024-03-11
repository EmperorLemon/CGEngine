#pragma once

#include "Renderer/Types.h"

#include <cstdint>

namespace CGEngine
{
	constexpr uint32_t CreateRGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255)
	{
		return ((r & 0xFF) << 24) + ((g & 0xFF) << 16) + ((b & 0xFF) << 8) + (a & 0xFF);
	}

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

		virtual void Enable(APICapability capability)  const = 0;
		virtual void Disable(APICapability capability) const = 0;

		virtual void Clear(ClearMask mask) = 0;
		virtual void ClearColor(const float* rgba) = 0;

		virtual void ResizeViewport(int32_t x, int32_t y, int32_t width, int32_t height) = 0;
	};
}
