#pragma once

#include "Renderer/Types.h"
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

	constexpr GLenum Convert(const FramebufferTextureAttachmentFormat format)
	{
		switch (format)
		{
		case FramebufferTextureAttachmentFormat::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
		}

		return GL_DEPTH24_STENCIL8;
	}

	constexpr GLenum Convert(const FramebufferTextureAttachment attachment)
	{
		switch (attachment)
		{
		case FramebufferTextureAttachment::COLOR_ATTACHMENT:		 return GL_COLOR_ATTACHMENT0;
		case FramebufferTextureAttachment::DEPTH_ATTACHMENT:		 return GL_DEPTH_ATTACHMENT;
		case FramebufferTextureAttachment::STENCIL_ATTACHMENT:		 return GL_STENCIL_ATTACHMENT;
		case FramebufferTextureAttachment::DEPTH_STENCIL_ATTACHMENT: return GL_DEPTH_STENCIL_ATTACHMENT;
		}

		return GL_COLOR_ATTACHMENT0;
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
		case DataType::DOUBLE:		   return GL_DOUBLE;
		}

		return GL_FLOAT;
	}

	constexpr GLenum Convert(const BufferTarget target)
	{
		switch (target)
		{
		case BufferTarget::NONE: break;
		case BufferTarget::VERTEX_BUFFER:		  return GL_ARRAY_BUFFER;
		case BufferTarget::INDEX_BUFFER:		  return GL_ELEMENT_ARRAY_BUFFER;
		case BufferTarget::UNIFORM_BUFFER:		  return GL_UNIFORM_BUFFER;
		case BufferTarget::SHADER_STORAGE_BUFFER: return GL_SHADER_STORAGE_BUFFER;
		case BufferTarget::FRAMEBUFFER:			  return GL_FRAMEBUFFER;
		case BufferTarget::RENDERBUFFER:		  return GL_RENDERBUFFER;
		}

		return GL_UNIFORM_BUFFER;
	}
}