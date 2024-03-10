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
		COLOR_ATTACHMENT0,
		COLOR_ATTACHMENT1,
		DEPTH_ATTACHMENT,
		STENCIL_ATTACHMENT
	};

	enum class BufferTarget : uint8_t
	{
		NONE,
		VERTEX_BUFFER,
		INDEX_BUFFER,
		UNIFORM_BUFFER,
		SHADER_STORAGE_BUFFER,
		FRAMEBUFFER
	};

	enum class TextureFormat : uint8_t
	{
		RED,
		RG,
		RGB,
		RGBA
	};

	enum class PixelFormat : uint8_t
	{
		RGBA8
	};
}