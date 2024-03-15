#include "OpenGLTexture.h"

#include "OpenGLTypes.hpp"
#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	constexpr TextureFormat GetTextureFormat(const PixelFormat pixelFormat)
	{
		switch (pixelFormat)
		{
		case PixelFormat::NONE:			return TextureFormat::NONE;
		case PixelFormat::R8:			return TextureFormat::RED;
		case PixelFormat::RG8:			return TextureFormat::RG;
		case PixelFormat::RGB:
		case PixelFormat::RGB8:			return TextureFormat::RGB;
		case PixelFormat::RGBA8:		return TextureFormat::RGBA;
		case PixelFormat::SRGB8:		return TextureFormat::SRGB;
		case PixelFormat::SRGB8_ALPHA8: return TextureFormat::SRGBA;
		}

		return TextureFormat::NONE;
	}

	GLTexture::GLTexture(const TextureTarget target, const int32_t levels, const PixelFormat pixelFormat, const int32_t width, const int32_t height, const TextureLayout& layout, const void* pixels) : Texture(width, height)
	{
		m_levels = levels;
		m_pixelFormat = pixelFormat;
		m_textureFormat = GetTextureFormat(pixelFormat);

		glCreateTextures(Convert(target), 1, &p_id);

		for (const auto& parameter : layout.GetParameters()) 
			glTextureParameteri(p_id, Convert(parameter.name), Convert(parameter.value));

		glTextureStorage2D(p_id, levels, Convert(pixelFormat), width, height);
		glTextureSubImage2D(p_id, 0, 0, 0, width, height, Convert(m_textureFormat), GL_UNSIGNED_BYTE, pixels);
		glGenerateTextureMipmap(p_id);
	}

	GLTexture::GLTexture(const TextureTarget target, const int32_t levels, const PixelFormat pixelFormat, const int32_t width, const int32_t height, const TextureLayout& layout) : Texture(width, height)
	{
		m_levels = levels;
		m_pixelFormat = pixelFormat;
		m_textureFormat = GetTextureFormat(pixelFormat);

		glCreateTextures(Convert(target), 1, &p_id);

		for (const auto& parameter : layout.GetParameters())
			glTextureParameteri(p_id, Convert(parameter.name), Convert(parameter.value));

		glTextureStorage2D(p_id, levels, Convert(pixelFormat), width, height);
	}

	GLTexture::GLTexture(const TextureTarget target, const int32_t levels, const TextureFormat textureFormat, const PixelFormat pixelFormat, const TextureLayout& layout, std::vector<Image>&& bitmaps) : Texture(0, 0)
	{
		m_levels = levels;
		m_pixelFormat = pixelFormat;
		m_textureFormat = textureFormat;

		glCreateTextures(Convert(target), 1, &p_id);

		for (const auto& parameter : layout.GetParameters())
			glTextureParameteri(p_id, Convert(parameter.name), Convert(parameter.value));

		if (!bitmaps.empty())
		{
			glTextureStorage2D(p_id, 1, Convert(pixelFormat), bitmaps.at(0).width, bitmaps.at(0).height);

			int face = 0;
			for (const auto& bitmap : bitmaps)
			{
				glTextureSubImage3D(p_id, 0, 0, 0, face, bitmap.width, bitmap.height, 1, Convert(textureFormat), GL_UNSIGNED_BYTE, bitmap.pixels.data());
				face++;
			}
		}
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &p_id);
	}

	void GLTexture::SetSubImage(const DataType type, const void* pixels) const
	{
		glTextureSubImage2D(p_id, 0, 0, 0, p_width, p_height, Convert(m_textureFormat), Convert(type), pixels);
		glGenerateTextureMipmap(p_id);
	}

	void GLTexture::ResizeImage(const int32_t width, const int32_t height) const
	{
		glTextureStorage2D(p_id, m_levels, Convert(m_pixelFormat), width, height);
	}

	void GLTexture::Bind(const uint32_t unit) const
	{
		glBindTextureUnit(unit, p_id);
	}

	void GLTexture::Unbind(const uint32_t unit) const
	{
		glBindTextureUnit(unit, 0);
	}
}
