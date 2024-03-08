#pragma once

#include <cstdint>

namespace CGEngine
{
	enum class TextureType : uint8_t
	{
		
	};

	class Texture
	{
	public:
		Texture();

		virtual void SetSubImage() const = 0;
	protected:
		uint32_t p_id = 0;
	};
}