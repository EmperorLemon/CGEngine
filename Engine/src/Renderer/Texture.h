#pragma once

#include "Types.h"

#include <cstdint>
#include <vector>

namespace CGEngine
{
	struct Image
	{
		int32_t width = 0;
		int32_t height = 0;
		int32_t channels = 0;
		std::vector<unsigned char> pixels;
	};

	class Texture
	{
	public:
		Texture(const int32_t width, const int32_t height) : p_width(width), p_height(height) {}

		Texture(Texture&&) noexcept = default;
		Texture(const Texture&) noexcept = delete;
		Texture& operator=(Texture&&) noexcept = delete;
		Texture& operator=(const Texture&) noexcept = delete;

		virtual ~Texture() = default;

		virtual void SetSubImage(TextureFormat format, DataType type, const void* pixels) const = 0;

		virtual void Bind(uint32_t unit)   const = 0;
		virtual void Unbind(uint32_t unit) const = 0;

		[[nodiscard]] virtual uint32_t GetID() const = 0;
	protected:
		uint32_t p_id = 0;

		int32_t p_width  = 0;
		int32_t p_height = 0;
	};
}