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
		case PixelFormat::RGB8:
		case PixelFormat::SRGB8:		return TextureFormat::RGB;
		case PixelFormat::RGBA8:
		case PixelFormat::SRGB8_ALPHA8: return TextureFormat::RGBA;
		case PixelFormat::DEPTH16:		
		case PixelFormat::DEPTH24:		
		case PixelFormat::DEPTH32F:		return TextureFormat::DEPTH;
		}

		return TextureFormat::NONE;
	}

	GLTexture::GLTexture(const TextureTarget target, const int32_t levels, const PixelFormat pixelFormat, const int32_t width, const int32_t height, const TextureLayout& layout, const void* pixels) : Texture(width, height)
	{
		m_levels = levels;
		m_pixelFormat = pixelFormat;
		m_textureFormat = GetTextureFormat(pixelFormat);

		glCreateTextures(Convert(target), 1, &m_id);

		for (const auto& parameter : layout.GetParameters()) 
			glTextureParameteri(m_id, Convert(parameter.name), Convert(parameter.value));

		glTextureStorage2D(m_id, levels, Convert(pixelFormat), width, height);
		glTextureSubImage2D(m_id, 0, 0, 0, width, height, Convert(m_textureFormat), GL_UNSIGNED_BYTE, pixels);
		glGenerateTextureMipmap(m_id);
	}

	GLTexture::GLTexture(const TextureTarget target, const int32_t levels, const PixelFormat pixelFormat, const int32_t width, const int32_t height, const TextureLayout& layout) : Texture(width, height)
	{
		m_levels = levels;
		m_pixelFormat = pixelFormat;
		m_textureFormat = GetTextureFormat(pixelFormat);

		glCreateTextures(Convert(target), 1, &m_id);

		for (const auto& parameter : layout.GetParameters())
			glTextureParameteri(m_id, Convert(parameter.name), Convert(parameter.value));

		glTextureStorage2D(m_id, levels, Convert(pixelFormat), width, height);
	}

	GLTexture::GLTexture(const TextureTarget target, const int32_t levels, const PixelFormat pixelFormat, const TextureLayout& layout, std::vector<Image>&& bitmaps) : Texture(0, 0)
	{
		m_levels = levels;
		m_pixelFormat = pixelFormat;
		m_textureFormat = GetTextureFormat(pixelFormat);

		glCreateTextures(Convert(target), 1, &m_id);

		for (const auto& parameter : layout.GetParameters())
			glTextureParameteri(m_id, Convert(parameter.name), Convert(parameter.value));

		if (!bitmaps.empty())
		{
			glTextureStorage2D(m_id, 1, Convert(pixelFormat), bitmaps.at(0).width, bitmaps.at(0).height);

			int face = 0;
			for (const auto& bitmap : bitmaps)
			{
				glTextureSubImage3D(m_id, 0, 0, 0, face, bitmap.width, bitmap.height, 1, Convert(m_textureFormat), GL_UNSIGNED_BYTE, bitmap.pixels.data());
				face++;
			}
		}
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &m_id);
	}

	void GLTexture::AddTextureParameter(const TParamName name, const float* value) const
	{
		glTextureParameterfv(m_id, Convert(name), value);
	}

	void GLTexture::Bind(const uint32_t unit) const
	{
		glBindTextureUnit(unit, m_id);
	}

	void GLTexture::Unbind(const uint32_t unit) const
	{
		glBindTextureUnit(unit, 0);
	}

	void GLTexture::ResizeImage(const int32_t width, const int32_t height) const
	{
		glTextureStorage2D(m_id, m_levels, Convert(m_pixelFormat), width, height);
	}

	void GLTexture::SetSubImage(const DataType type, const void* pixels) const
	{
		glTextureSubImage2D(m_id, 0, 0, 0, p_width, p_height, Convert(m_textureFormat), Convert(type), pixels);
		glGenerateTextureMipmap(m_id);
	}
}
