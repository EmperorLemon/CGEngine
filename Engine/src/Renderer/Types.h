#pragma once

#include <cstdint>

namespace CGEngine
{
	enum class BufferMask : uint8_t
	{
		COLOR_BUFFER_BIT,
		DEPTH_BUFFER_BIT,
		COLOR_DEPTH_BUFFER_BIT,
		STENCIL_BUFFER_BIT
	};

	enum class APICapability : uint8_t
	{
		BLEND,
		CULL_FACE,
		DEPTH_TEST,
		STENCIL_TEST,
		FRAMEBUFFER_SRGB,
		LINE_SMOOTH,
		POLYGON_SMOOTH,
		SCISSOR_TEST,
	};

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

	enum class BufferType : uint8_t
	{
		COLOR,
		DEPTH,
		STENCIL,
		DEPTH_STENCIL
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

	enum class FramebufferTarget : uint8_t
	{
		FRAMEBUFFER,
		READ_FRAMEBUFFER,
		DRAW_FRAMEBUFFER
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

	enum class TextureTarget : uint8_t
	{
		TEXTURE_2D,
		TEXTURE_3D
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

	enum class TextureFilter : uint8_t
	{
		NEAREST,
		LINEAR
	};

	enum class PixelFormat : uint8_t
	{
		RGB,
		RGB8,
		RGBA8
	};

	enum class DrawType : uint8_t
	{
		DRAW_ARRAYS,
		DRAW_ELEMENTS
	};
}