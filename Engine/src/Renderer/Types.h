#pragma once

#include <cstdint>

namespace CGEngine
{
	enum class BufferMask : uint8_t
	{
		NONE = 0,
		COLOR_BUFFER_BIT,
		DEPTH_BUFFER_BIT,
		COLOR_DEPTH_BUFFER_BIT,
		STENCIL_BUFFER_BIT
	};

	enum class DepthFunc : uint8_t
	{
		NONE = 0,
		LESS,
		LEQUAL,
		EQUAL
	};

	enum class APICapability : uint8_t
	{
		NONE = 0,
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
		NONE = 0,
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
		NONE = 0,
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
		NONE = 0,
		COLOR,
		DEPTH,
		STENCIL,
		DEPTH_STENCIL
	};

	enum class BufferTarget : uint8_t
	{
		NONE = 0,
		VERTEX_BUFFER,
		INDEX_BUFFER,
		UNIFORM_BUFFER,
		SHADER_STORAGE_BUFFER,
		FRAMEBUFFER,
		RENDERBUFFER
	};

	enum class FramebufferTarget : uint8_t
	{
		NONE = 0,
		FRAMEBUFFER,
		READ_FRAMEBUFFER,
		DRAW_FRAMEBUFFER
	};

	enum class FramebufferTextureAttachment : uint8_t
	{
		NONE = 0,
		COLOR_ATTACHMENT,
		DEPTH_ATTACHMENT,
		STENCIL_ATTACHMENT,
		DEPTH_STENCIL_ATTACHMENT
	};

	enum class FramebufferTextureAttachmentFormat : uint8_t
	{
		NONE = 0,
		DEPTH24_STENCIL8
	};

	enum class TextureTarget : uint8_t
	{
		NONE = 0,
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_CUBE_MAP
	};

	enum class TextureFormat : uint8_t
	{
		NONE = 0,
		RED,
		RG,
		RGB,
		RGBA,
		SRGB,
		SRGBA
	};

	enum class TParamName : uint8_t
	{
		NONE = 0,
		TEXTURE_WRAP_S,
		TEXTURE_WRAP_T,
		TEXTURE_WRAP_R,
		TEXTURE_MIN_FILTER,
		TEXTURE_MAG_FILTER
	};

	enum class TParamValue : uint8_t
	{
		NONE = 0,
		CLAMP_TO_EDGE,
		REPEAT,
		NEAREST,
		LINEAR
	};

	enum class PixelFormat : uint8_t
	{
		NONE = 0,
		RGB,
		RGB8,
		RGBA8
	};

	enum class PolygonMode : uint8_t
	{
		NONE = 0,
		WIREFRAME,
		SOLID
	};

	enum class DrawType : uint8_t
	{
		NONE = 0,
		DRAW_ARRAYS,
		DRAW_ELEMENTS
	};
}