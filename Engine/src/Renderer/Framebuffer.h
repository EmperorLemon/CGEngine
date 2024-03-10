#pragma once

#include <cstdint>

namespace CGEngine
{
	class Framebuffer
	{
	public:
		Framebuffer() = default;

		Framebuffer(Framebuffer&&) noexcept = default;
		Framebuffer(const Framebuffer&) noexcept = delete;
		Framebuffer& operator=(Framebuffer&&) noexcept = delete;
		Framebuffer& operator=(const Framebuffer&) noexcept = delete;

		virtual ~Framebuffer() = default;
	protected:
		uint32_t p_id = 0;
	};
}