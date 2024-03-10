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
	private:

	};
}