#pragma once

#include "Renderer/Texture.h"

namespace CGEngine::OpenGL
{
	class GLTexture final : public Texture
	{
	public:
		GLTexture(TextureTarget target, int32_t levels, PixelFormat format, int32_t width, int32_t height, const TextureLayout& layout, const void* pixels);
		GLTexture(TextureTarget target, int32_t levels, PixelFormat format, int32_t width, int32_t height, const TextureLayout& layout);
		GLTexture(TextureTarget target, int32_t levels, TextureFormat format, PixelFormat internalFormat,  const TextureLayout& layout, std::vector<Image>&& bitmaps);

		GLTexture(GLTexture&&) noexcept = default;
		GLTexture(const GLTexture&) noexcept = delete;
		GLTexture& operator=(GLTexture&&) noexcept = delete;
		GLTexture& operator=(const GLTexture&) noexcept = delete;

		~GLTexture() override;

		void SetSubImage(TextureFormat format, DataType type, const void* pixels) const override;
		void ResizeImage(int32_t width, int32_t height) const;

		void Bind(uint32_t unit) const override;
		void Unbind(uint32_t unit) const override;

		[[nodiscard]] uint32_t GetID() const override { return p_id; }
	private:
		int32_t m_levels = 1;
		PixelFormat m_format = PixelFormat::NONE;
	};
}