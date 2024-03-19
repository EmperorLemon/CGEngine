#pragma once

#include "Types.h"

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

		virtual void AttachTexture(FramebufferTextureAttachment attachment, uint32_t texture) const = 0;
		virtual void AttachRenderbuffer(FramebufferTextureAttachment attachment, uint32_t renderbuffer) const = 0;

		virtual void Bind(FramebufferTarget target) const = 0;
		virtual void Unbind(FramebufferTarget target) const = 0;

		virtual void DrawBuffer() const = 0;
		virtual void ReadBuffer() const = 0;

		virtual void Clear(BufferType type, int32_t index, const float* value) const = 0;

		[[nodiscard]] virtual uint32_t GetID() const = 0;
	};

	class Renderbuffer
	{
	public:
		Renderbuffer() = default;

		Renderbuffer(Renderbuffer&&) noexcept = default;
		Renderbuffer(const Renderbuffer&) noexcept = delete;
		Renderbuffer& operator=(Renderbuffer&&) noexcept = delete;
		Renderbuffer& operator=(const Renderbuffer&) noexcept = delete;

		virtual ~Renderbuffer() = default;

		[[nodiscard]] virtual uint32_t GetID() const = 0;
	};
}