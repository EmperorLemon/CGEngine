#pragma once

#include <cstdint>

namespace CGEngine
{
	enum class DataType : uint8_t
	{
		VOID,
		UNSIGNED_BYTE,
		BYTE,
		UNSIGNED_SHORT,
		SHORT,
		UNSIGNED_INT,
		INT,
		FLOAT,
		DOUBLE
	};

	enum class ShaderType : uint8_t
	{
		NONE,
		VERTEX,
		GEOMETRY,
		TESSELATION_CONTROL,
		TESSELATION_EVALUATION,
		FRAGMENT,
		COMPUTE,
		PROGRAM,
	};

	enum class TextureTarget : uint8_t
	{
		TEXTURE_2D,
		TEXTURE_3D
	};

	enum class FramebufferTextureAttachment : uint8_t
	{
		COLOR_ATTACHMENT,
		DEPTH_ATTACHMENT,
		STENCIL_ATTACHMENT,
		DEPTH_STENCIL_ATTACHMENT
	};

	enum class FramebufferTextureAttachmentFormat : uint8_t
	{
		DEPTH24_STENCIL8
	};

	enum class BufferTarget : uint8_t
	{
		NONE,
		VERTEX_BUFFER,
		INDEX_BUFFER,
		UNIFORM_BUFFER,
		SHADER_STORAGE_BUFFER,
		FRAMEBUFFER,
		RENDERBUFFER
	};

	enum class TextureFormat : uint8_t
	{
		RED,
		RG,
		RGB,
		RGBA,
		SRGB,
		SRGBA
	};

	enum class PixelFormat : uint8_t
	{
		RGB,
		RGB8,
		RGBA8
	};
}