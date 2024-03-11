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
	};
}