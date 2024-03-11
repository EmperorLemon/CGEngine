#pragma once

#include "Renderer/Types.h"
#include <glad/gl.h>

namespace CGEngine::OpenGL
{
	constexpr GLenum Convert(const ClearMask mask)
	{
		switch (mask)
		{
		case ClearMask::NULL_BUFFER_BIT:		break;
		case ClearMask::COLOR_BUFFER_BIT:	    return GL_COLOR_BUFFER_BIT;
		case ClearMask::DEPTH_BUFFER_BIT:       return GL_DEPTH_BUFFER_BIT;
		case ClearMask::COLOR_DEPTH_BUFFER_BIT: return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		case ClearMask::STENCIL_BUFFER_BIT:	    return GL_STENCIL_BUFFER_BIT;
		}

		return 0;
	}
	constexpr GLenum Convert(const APICapability capability)
	{
		switch (capability)
		{
		case APICapability::BLEND:			  return GL_BLEND;
		case APICapability::CULL_FACE:		  return GL_CULL_FACE;
		case APICapability::DEPTH_TEST:		  return GL_DEPTH_TEST;
		case APICapability::STENCIL_TEST:     return GL_STENCIL_TEST;
		case APICapability::FRAMEBUFFER_SRGB: return GL_FRAMEBUFFER_SRGB;
		case APICapability::LINE_SMOOTH:	  return GL_LINE_SMOOTH;
		case APICapability::POLYGON_SMOOTH:   return GL_POLYGON_SMOOTH;
		case APICapability::SCISSOR_TEST:     return GL_SCISSOR_TEST;
		}

		return 0;
	}

	constexpr GLenum Convert(const TextureTarget target)
	{
		switch (target)
		{
		case TextureTarget::TEXTURE_2D: return GL_TEXTURE_2D;
		case TextureTarget::TEXTURE_3D: return GL_TEXTURE_3D;
		}

		return 0;
	}

	constexpr GLenum Convert(const TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RED:   return GL_RED;
		case TextureFormat::RG:    return GL_RG;
		case TextureFormat::RGB:   return GL_RGB;
		case TextureFormat::RGBA:  return GL_RGBA;
		case TextureFormat::SRGB:  return GL_SRGB;
		case TextureFormat::SRGBA: return GL_SRGB_ALPHA;
		}

		return 0;
	}

	constexpr GLenum Convert(const FramebufferTextureAttachmentFormat format)
	{
		switch (format)
		{
		case FramebufferTextureAttachmentFormat::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
		}

		return 0;
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

		return 0;
	}

	constexpr GLenum Convert(const PixelFormat format)
	{
		switch (format)
		{
		case PixelFormat::RGB:	 return GL_RGB;
		case PixelFormat::RGB8:  return GL_RGB8;
		case PixelFormat::RGBA8: return GL_RGBA8;
		}

		return 0;
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

		return 0;
	}

	constexpr GLenum Convert(const BufferType type)
	{
		switch (type)
		{
		case BufferType::COLOR:   return GL_COLOR;
		case BufferType::DEPTH:   return GL_DEPTH;
		case BufferType::STENCIL: return GL_STENCIL;
		}

		return 0;
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

		return 0;
	}

	constexpr GLenum Convert(const FramebufferTarget target)
	{
		switch (target)
		{
		case FramebufferTarget::FRAMEBUFFER:	    return GL_FRAMEBUFFER;
		case FramebufferTarget::READ_FRAMEBUFFER: return GL_READ_FRAMEBUFFER;
		case FramebufferTarget::DRAW_FRAMEBUFFER: return GL_DRAW_FRAMEBUFFER;
		}

		return 0;
	}

}