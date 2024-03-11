#pragma once

#include "Types.h"

#include <cstdint>

namespace CGEngine
{
	class Framebuffer
	{
	public:
		explicit Framebuffer(const BufferTarget target) : p_target(target) {}

		Framebuffer(Framebuffer&&) noexcept = default;
		Framebuffer(const Framebuffer&) noexcept = delete;
		Framebuffer& operator=(Framebuffer&&) noexcept = delete;
		Framebuffer& operator=(const Framebuffer&) noexcept = delete;

		virtual ~Framebuffer() = default;

		virtual void AttachTexture(FramebufferTextureAttachment attachment, uint32_t texture) const = 0;
		virtual void AttachRenderbuffer(FramebufferTextureAttachment attachment, uint32_t renderbuffer) const = 0;

		virtual void Bind(FramebufferTarget target) const = 0;
		virtual void Unbind(FramebufferTarget target) const = 0;

		virtual void Clear(BufferType type, int32_t index, const float* value) const = 0;
	protected:
		uint32_t p_id = 0;
		BufferTarget p_target = BufferTarget::NONE;
	};

	class Renderbuffer
	{
	public:
		explicit Renderbuffer(const BufferTarget target) : p_target(target) {}

		Renderbuffer(Renderbuffer&&) noexcept = default;
		Renderbuffer(const Renderbuffer&) noexcept = delete;
		Renderbuffer& operator=(Renderbuffer&&) noexcept = delete;
		Renderbuffer& operator=(const Renderbuffer&) noexcept = delete;

		virtual ~Renderbuffer() = default;

		[[nodiscard]] virtual uint32_t GetID() const = 0;
	protected:
		uint32_t p_id = 0;
		BufferTarget p_target = BufferTarget::NONE;
	};
}