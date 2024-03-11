#pragma once

#include "Renderer/Framebuffer.h"

namespace CGEngine::OpenGL
{
	class GLFramebuffer final : public Framebuffer
	{
	public:
		explicit GLFramebuffer(BufferTarget target);

		GLFramebuffer(GLFramebuffer&&) noexcept = default;
		GLFramebuffer(const GLFramebuffer&) noexcept = delete;
		GLFramebuffer& operator=(GLFramebuffer&&) noexcept = delete;
		GLFramebuffer& operator=(const GLFramebuffer&) noexcept = delete;

		~GLFramebuffer() override;

		void AttachTexture(FramebufferTextureAttachment attachment, uint32_t texture) const override;
		void AttachRenderbuffer(FramebufferTextureAttachment attachment, uint32_t renderbuffer) const override;

		void Bind(FramebufferTarget target = FramebufferTarget::FRAMEBUFFER) const override;
		void Unbind(FramebufferTarget target = FramebufferTarget::FRAMEBUFFER) const override;

		void Clear(BufferType type, int32_t index, const float* value) const override;

		[[nodiscard]] bool CheckStatus() const;
	};

	class GLRenderbuffer final : public Renderbuffer
	{
	public:
		explicit GLRenderbuffer(BufferTarget target, FramebufferTextureAttachmentFormat format, int32_t width, int32_t height);

		GLRenderbuffer(GLRenderbuffer&&) noexcept = default;
		GLRenderbuffer(const GLRenderbuffer&) noexcept = delete;
		GLRenderbuffer& operator=(GLRenderbuffer&&) noexcept = delete;
		GLRenderbuffer& operator=(const GLRenderbuffer&) noexcept = delete;

		~GLRenderbuffer() override;

		[[nodiscard]] uint32_t GetID() const override { return p_id; }
	};
}