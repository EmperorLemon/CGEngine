#pragma once

#include "Renderer/Framebuffer.h"

namespace CGEngine::OpenGL
{
	class GLFramebuffer : public Framebuffer
	{
	public:
		GLFramebuffer();

		GLFramebuffer(GLFramebuffer&&) noexcept = default;
		GLFramebuffer(const GLFramebuffer&) noexcept = delete;
		GLFramebuffer& operator=(GLFramebuffer&&) noexcept = delete;
		GLFramebuffer& operator=(const GLFramebuffer&) noexcept = delete;

		~GLFramebuffer() override;

		bool CheckStatus(uint32_t target);


		void AddFramebufferTexture();
	};
}