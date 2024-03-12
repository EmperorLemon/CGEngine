#include "OpenGLTexture.h"

#include "OpenGLTypes.hpp"
#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	GLTexture::GLTexture(const TextureTarget target, const int32_t levels, const PixelFormat format, const int32_t width, const int32_t height, const TextureLayout& layout, const void* pixels) : Texture(width, height)
	{
		m_levels = levels;
		m_format = format;

		glCreateTextures(Convert(target), 1, &p_id);

		for (const auto& parameter : layout.GetParameters()) 
			glTextureParameteri(p_id, Convert(parameter.name), Convert(parameter.value));

		glTextureStorage2D(p_id, levels, Convert(format), width, height);
		glTextureSubImage2D(p_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	GLTexture::GLTexture(const TextureTarget target, const int32_t levels, const PixelFormat format, const int32_t width, const int32_t height, const TextureLayout& layout) : Texture(width, height)
	{
		m_levels = levels;
		m_format = format;

		glCreateTextures(Convert(target), 1, &p_id);

		for (const auto& parameter : layout.GetParameters())
			glTextureParameteri(p_id, Convert(parameter.name), Convert(parameter.value));

		glTextureStorage2D(p_id, levels, Convert(format), width, height);
	}

	GLTexture::GLTexture(const TextureTarget target, const int32_t levels, const TextureFormat format, const PixelFormat internalFormat, const TextureLayout& layout, std::vector<Image>&& bitmaps) : Texture(0, 0)
	{
		m_levels = levels;
		m_format = internalFormat;

		glCreateTextures(Convert(target), 1, &p_id);

		for (const auto& parameter : layout.GetParameters())
			glTextureParameteri(p_id, Convert(parameter.name), Convert(parameter.value));

		if (!bitmaps.empty())
		{
			glTextureStorage2D(p_id, 1, Convert(internalFormat), bitmaps.at(0).width, bitmaps.at(0).height);

			int face = 0;
			for (const auto& bitmap : bitmaps)
			{
				glTextureSubImage3D(p_id, 0, 0, 0, face, bitmap.width, bitmap.height, 1, Convert(format), GL_UNSIGNED_BYTE, bitmap.pixels.data());
				face++;
			}
		}
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &p_id);
	}

	void GLTexture::SetSubImage(const TextureFormat format, const DataType type, const void* pixels) const
	{
		glTextureSubImage2D(p_id, 0, 0, 0, p_width, p_height, Convert(format), Convert(type), pixels);
	}

	void GLTexture::ResizeImage(const int32_t width, const int32_t height) const
	{
		glTextureStorage2D(p_id, m_levels, Convert(m_format), width, height);
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
