#pragma once

#include "Renderer/Texture.h"

namespace CGEngine::OpenGL
{
	class GLTexture final : public Texture
	{
	public:
		GLTexture(TextureTarget target, int32_t levels, PixelFormat pixelFormat, int32_t width, int32_t height, const TextureLayout& layout, const void* pixels);
		GLTexture(TextureTarget target, int32_t levels, PixelFormat pixelFormat, int32_t width, int32_t height, const TextureLayout& layout);
		GLTexture(TextureTarget target, int32_t levels, PixelFormat pixelFormat, const TextureLayout& layout, std::vector<Image>&& bitmaps);

		GLTexture(GLTexture&&) noexcept = default;
		GLTexture(const GLTexture&) noexcept = delete;
		GLTexture& operator=(GLTexture&&) noexcept = delete;
		GLTexture& operator=(const GLTexture&) noexcept = delete;

		~GLTexture() override;

		void AddTextureParameter(TParamName name, const float* value) const;

		void Bind(uint32_t unit = 0) const override;
		void Unbind(uint32_t unit = 0) const override;

		void ResizeImage(int32_t width, int32_t height) const;
		void SetSubImage(DataType type, const void* pixels) const override;

		[[nodiscard]] uint32_t GetID() const override { return m_id; }
	private:
		int32_t  m_levels = 1;
		uint32_t m_id = 0;

		PixelFormat m_pixelFormat = PixelFormat::NONE;
		TextureFormat m_textureFormat = TextureFormat::NONE;
	};
}