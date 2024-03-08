#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace CGEngine::OpenGL
{
	constexpr GLenum Convert(const TextureTarget target)
	{
		switch (target)
		{
		case TextureTarget::TEXTURE_2D: return GL_TEXTURE_2D;
		case TextureTarget::TEXTURE_3D: return GL_TEXTURE_3D;
		}

		return GL_TEXTURE_2D;
	}

	constexpr GLenum Convert(const TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RED:  return GL_RED;
		case TextureFormat::RG:   return GL_RG;
		case TextureFormat::RGB:  return GL_RGB;
		case TextureFormat::RGBA: return GL_RGBA;
		}

		return GL_RGBA;
	}

	constexpr GLenum Convert(const PixelFormat format)
	{
		switch (format)
		{
		case PixelFormat::RGBA8: return GL_RGBA8;
		}

		return GL_RGBA8;
	}

	constexpr GLenum Convert(const DataType type)
	{
		switch (type)
		{
		case DataType::VOID:
		case DataType::UNSIGNED_BYTE:  return GL_UNSIGNED_BYTE;
		case DataType::BYTE:		   return GL_BYTE;
		case DataType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
		case DataType::SHORT:		   return GL_SHORT;
		case DataType::UNSIGNED_INT:   return GL_UNSIGNED_INT;
		case DataType::INT:            return GL_INT;
		case DataType::FLOAT:	       return GL_FLOAT;
		}

		return GL_UNSIGNED_BYTE;
	}

	GLTexture::GLTexture(const TextureTarget target, const PixelFormat format, const int32_t width, const int32_t height) : Texture(width, height)
	{
		glCreateTextures(Convert(target), 1, &p_id);

		glTextureParameteri(p_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(p_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(p_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(p_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureStorage2D(p_id, 1, Convert(format), width, height);
	}

	GLTexture::~GLTexture()
	{
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