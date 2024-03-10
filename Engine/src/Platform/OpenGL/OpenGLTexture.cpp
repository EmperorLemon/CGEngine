#include "OpenGLTexture.h"

#include "OpenGLTypes.hpp"
#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	GLTexture::GLTexture(const TextureTarget target, const PixelFormat format, const int32_t width, const int32_t height, const void* pixels) : Texture(width, height)
	{
		glCreateTextures(Convert(target), 1, &p_id);

		glTextureParameteri(p_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(p_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(p_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(p_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureStorage2D(p_id, 1, Convert(format), width, height);
		glTextureSubImage2D(p_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	GLTexture::GLTexture(const TextureTarget target, const PixelFormat format, const int32_t width, const int32_t height) : Texture(width, height)
	{
		glCreateTextures(Convert(target), 1, &p_id);

		glTextureParameteri(p_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(p_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(p_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(p_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureStorage2D(p_id, 1, Convert(format), width, height);
	}

	GLTexture::~GLTexture()
	{
		CG_TRACE("Deleted GLTexture {0}", p_id);
		glDeleteTextures(1, &p_id);
	}

	void GLTexture::SetSubImage(const TextureFormat format, const DataType type, const void* pixels) const
	{
		glTextureSubImage2D(p_id, 0, 0, 0, p_width, p_height, Convert(format), Convert(type), pixels);
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
