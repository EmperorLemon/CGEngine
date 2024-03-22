#pragma once

#include "Renderer/Framebuffer.h"

#include "Math/Vector4.h"

namespace CGEngine::OpenGL
{
	class GLFramebuffer final : public Framebuffer
	{
	public:
		explicit GLFramebuffer();

		GLFramebuffer(GLFramebuffer&&) noexcept = default;
		GLFramebuffer(const GLFramebuffer&) noexcept = delete;
		GLFramebuffer& operator=(GLFramebuffer&&) noexcept = delete;
		GLFramebuffer& operator=(const GLFramebuffer&) noexcept = delete;

		~GLFramebuffer() override;

		void AttachTexture(FramebufferTextureAttachment attachment, uint32_t texture, int32_t index = 0) const override;
		void AttachRenderbuffer(FramebufferTextureAttachment attachment, uint32_t renderbuffer) const override;

		void Bind(FramebufferTarget target = FramebufferTarget::FRAMEBUFFER) const override;
		void Unbind(FramebufferTarget target = FramebufferTarget::FRAMEBUFFER) const override;

		void Blit(uint32_t otherID, const Math::IVec4& src, const Math::IVec4& dst, BufferMask mask, TParamValue filter) const;

		void Clear(BufferType type, int32_t index, const float* value) const override;

		void DrawBuffer() const override;
		void ReadBuffer() const override;

		void DrawBuffers(int32_t count = 2) const override;

		[[nodiscard]] bool CheckStatus() const;

		[[nodiscard]] uint32_t GetID() const override { return m_id; }
	private:
		uint32_t m_id = 0;
	};

	class GLRenderbuffer final : public Renderbuffer
	{
	public:
		explicit GLRenderbuffer(FramebufferTextureAttachmentFormat format, int32_t width, int32_t height);

		GLRenderbuffer(GLRenderbuffer&&) noexcept = default;
		GLRenderbuffer(const GLRenderbuffer&) noexcept = delete;
		GLRenderbuffer& operator=(GLRenderbuffer&&) noexcept = delete;
		GLRenderbuffer& operator=(const GLRenderbuffer&) noexcept = delete;

		~GLRenderbuffer() override;

		void ResizeBuffer(int32_t width, int32_t height) const;

		[[nodiscard]] uint32_t GetID() const override { return m_id; }

	private:
		FramebufferTextureAttachmentFormat m_format = FramebufferTextureAttachmentFormat::NONE;
		uint32_t m_id = 0;
	};
}