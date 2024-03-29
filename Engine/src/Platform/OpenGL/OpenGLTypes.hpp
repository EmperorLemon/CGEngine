#pragma once

#include "Renderer/Types.h"
#include <glad/gl.h>

namespace CGEngine::OpenGL
{
	constexpr GLbitfield Convert(const BufferMask mask)
	{
		switch (mask)
		{
		case BufferMask::NONE:					 return GL_NONE;
		case BufferMask::COLOR_BUFFER_BIT:	     return GL_COLOR_BUFFER_BIT;
		case BufferMask::DEPTH_BUFFER_BIT:       return GL_DEPTH_BUFFER_BIT;
		case BufferMask::COLOR_DEPTH_BUFFER_BIT: return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		case BufferMask::STENCIL_BUFFER_BIT:	 return GL_STENCIL_BUFFER_BIT;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const DepthFunc func)
	{
		switch (func)
		{
		case DepthFunc::NONE:	return GL_NONE;
		case DepthFunc::LESS:   return GL_LESS;
		case DepthFunc::LEQUAL: return GL_LEQUAL;
		case DepthFunc::EQUAL:  return GL_EQUAL;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const CullFace face)
	{
		switch (face)
		{
		case CullFace::NONE:			return GL_NONE;
		case CullFace::FRONT:			return GL_FRONT;
		case CullFace::BACK:			return GL_BACK;
		case CullFace::FRONT_AND_BACK:  return GL_FRONT_AND_BACK;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const APICapability capability)
	{
		switch (capability)
		{
		case APICapability::NONE:			  return GL_NONE;
		case APICapability::BLEND:			  return GL_BLEND;
		case APICapability::CULL_FACE:		  return GL_CULL_FACE;
		case APICapability::DEPTH_TEST:		  return GL_DEPTH_TEST;
		case APICapability::STENCIL_TEST:     return GL_STENCIL_TEST;
		case APICapability::FRAMEBUFFER_SRGB: return GL_FRAMEBUFFER_SRGB;
		case APICapability::LINE_SMOOTH:	  return GL_LINE_SMOOTH;
		case APICapability::POLYGON_SMOOTH:   return GL_POLYGON_SMOOTH;
		case APICapability::SCISSOR_TEST:     return GL_SCISSOR_TEST;
		case APICapability::MULTISAMPLING:	  return GL_MULTISAMPLE;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const PolygonMode mode)
	{
		switch (mode)
		{
		case PolygonMode::NONE:		 return GL_NONE;
		case PolygonMode::WIREFRAME: return GL_LINE;
		case PolygonMode::SOLID:	 return GL_FILL;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const TextureTarget target)
	{
		switch (target)
		{
		case TextureTarget::NONE:			  return GL_NONE;
		case TextureTarget::TEXTURE_2D:		  return GL_TEXTURE_2D;
		case TextureTarget::TEXTURE_3D:		  return GL_TEXTURE_3D;
		case TextureTarget::TEXTURE_CUBE_MAP: return GL_TEXTURE_CUBE_MAP;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::NONE:    return GL_NONE;
		case TextureFormat::RED:     return GL_RED;
		case TextureFormat::RG:      return GL_RG;
		case TextureFormat::RGB:     return GL_RGB;
		case TextureFormat::BGR:	 return GL_BGR;
		case TextureFormat::RGBA:    return GL_RGBA;
		case TextureFormat::BGRA:	 return GL_BGRA;
		case TextureFormat::DEPTH:   return GL_DEPTH_COMPONENT;
		case TextureFormat::STENCIL: return GL_STENCIL_INDEX;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const TParamName name)
	{
		switch (name)
		{
		case TParamName::NONE:				   return GL_NONE;
		case TParamName::TEXTURE_WRAP_S:	   return GL_TEXTURE_WRAP_S;
		case TParamName::TEXTURE_WRAP_T:	   return GL_TEXTURE_WRAP_T;
		case TParamName::TEXTURE_WRAP_R:	   return GL_TEXTURE_WRAP_R;
		case TParamName::TEXTURE_MIN_FILTER:   return GL_TEXTURE_MIN_FILTER;
		case TParamName::TEXTURE_MAG_FILTER:   return GL_TEXTURE_MAG_FILTER;
		case TParamName::TEXTURE_BORDER_COLOR: return GL_TEXTURE_BORDER_COLOR;
		}

		return GL_NONE;
	}

	constexpr GLint Convert(const TParamValue value)
	{
		switch (value)
		{
		case TParamValue::NONE:					  return GL_NONE;
		case TParamValue::CLAMP_TO_EDGE:		  return GL_CLAMP_TO_EDGE;
		case TParamValue::CLAMP_TO_BORDER:		  return GL_CLAMP_TO_BORDER;
		case TParamValue::REPEAT:				  return GL_REPEAT;
		case TParamValue::MIRRORED_REPEAT:		  return GL_MIRRORED_REPEAT;
		case TParamValue::NEAREST:				  return GL_NEAREST;
		case TParamValue::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
		case TParamValue::NEAREST_MIPMAP_LINEAR:  return GL_NEAREST_MIPMAP_LINEAR;
		case TParamValue::LINEAR:				  return GL_LINEAR;
		case TParamValue::LINEAR_MIPMAP_NEAREST:  return GL_LINEAR_MIPMAP_NEAREST;
		case TParamValue::LINEAR_MIPMAP_LINEAR:   return GL_LINEAR_MIPMAP_LINEAR;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const FramebufferTextureAttachmentFormat format)
	{
		switch (format)
		{
		case FramebufferTextureAttachmentFormat::NONE:				return GL_NONE;
		case FramebufferTextureAttachmentFormat::DEPTH16:			return GL_DEPTH_COMPONENT16;
		case FramebufferTextureAttachmentFormat::DEPTH24:			return GL_DEPTH_COMPONENT24;
		case FramebufferTextureAttachmentFormat::DEPTH32:			return GL_DEPTH_COMPONENT32;
		case FramebufferTextureAttachmentFormat::DEPTH32F:			return GL_DEPTH_COMPONENT32F;
		case FramebufferTextureAttachmentFormat::STENCIL_INDEX8:	return GL_STENCIL_INDEX8;
		case FramebufferTextureAttachmentFormat::DEPTH24_STENCIL8:	return GL_DEPTH24_STENCIL8;
		case FramebufferTextureAttachmentFormat::DEPTH32F_STENCIL8: return GL_DEPTH32F_STENCIL8;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const FramebufferTextureAttachment attachment)
	{
		switch (attachment)
		{
		case FramebufferTextureAttachment::NONE:					 return GL_NONE;
		case FramebufferTextureAttachment::COLOR_ATTACHMENT:		 return GL_COLOR_ATTACHMENT0;
		case FramebufferTextureAttachment::DEPTH_ATTACHMENT:		 return GL_DEPTH_ATTACHMENT;
		case FramebufferTextureAttachment::STENCIL_ATTACHMENT:		 return GL_STENCIL_ATTACHMENT;
		case FramebufferTextureAttachment::DEPTH_STENCIL_ATTACHMENT: return GL_DEPTH_STENCIL_ATTACHMENT;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const PixelFormat format)
	{
		switch (format)
		{
		case PixelFormat::NONE:			return GL_NONE;
		case PixelFormat::R8:			return GL_R8;
		case PixelFormat::R16F:			return GL_R16F;
		case PixelFormat::R32F:			return GL_R32F;
		case PixelFormat::RG8:			return GL_RG8;
		case PixelFormat::RG16F:		return GL_RG16F;
		case PixelFormat::RG32F:		return GL_RG32F;
		case PixelFormat::RGB8:			return GL_RGB8;
		case PixelFormat::RGB16F:		return GL_RGB16F;
		case PixelFormat::RGB32F:		return GL_RGB32F;
		case PixelFormat::RGBA8:		return GL_RGBA8;
		case PixelFormat::RGBA16F:		return GL_RGBA16F;
		case PixelFormat::RGBA32F:		return GL_RGBA32F;
		case PixelFormat::SRGB8:		return GL_SRGB8;
		case PixelFormat::SRGB8_ALPHA8: return GL_SRGB8_ALPHA8;
		case PixelFormat::DEPTH16:		return GL_DEPTH_COMPONENT16;
		case PixelFormat::DEPTH24:		return GL_DEPTH_COMPONENT24;
		case PixelFormat::DEPTH32F:		return GL_DEPTH_COMPONENT32F;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const DataType type)
	{
		switch (type)
		{
		case DataType::NONE:		   return GL_NONE;
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

		return GL_NONE;
	}

	constexpr GLenum Convert(const BufferType type)
	{
		switch (type)
		{
		case BufferType::NONE:			return GL_NONE;
		case BufferType::COLOR:			return GL_COLOR;
		case BufferType::DEPTH:			return GL_DEPTH;
		case BufferType::STENCIL:		return GL_STENCIL;
		case BufferType::DEPTH_STENCIL: return GL_DEPTH_STENCIL;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const BufferTarget target)
	{
		switch (target)
		{
		case BufferTarget::NONE:				  return GL_NONE;
		case BufferTarget::VERTEX_BUFFER:		  return GL_ARRAY_BUFFER;
		case BufferTarget::INDEX_BUFFER:		  return GL_ELEMENT_ARRAY_BUFFER;
		case BufferTarget::INSTANCE_BUFFER:		  return GL_NONE;
		case BufferTarget::UNIFORM_BUFFER:		  return GL_UNIFORM_BUFFER;
		case BufferTarget::SHADER_STORAGE_BUFFER: return GL_SHADER_STORAGE_BUFFER;
		case BufferTarget::FRAMEBUFFER:			  return GL_FRAMEBUFFER;
		case BufferTarget::RENDERBUFFER:		  return GL_RENDERBUFFER;
		}

		return GL_NONE;
	}

	constexpr GLenum Convert(const FramebufferTarget target)
	{
		switch (target)
		{
		case FramebufferTarget::NONE:			  return GL_NONE;
		case FramebufferTarget::FRAMEBUFFER:	  return GL_FRAMEBUFFER;
		case FramebufferTarget::READ_FRAMEBUFFER: return GL_READ_FRAMEBUFFER;
		case FramebufferTarget::DRAW_FRAMEBUFFER: return GL_DRAW_FRAMEBUFFER;
		}

		return GL_NONE;
	}

}