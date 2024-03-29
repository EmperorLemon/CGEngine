#pragma once

#include "Types.h"

#include <cstdint>
#include <vector>

namespace CGEngine
{
	struct TextureParameter
	{
		TParamName  name  =  TParamName::NONE;
		TParamValue value =  TParamValue::NONE;
	};

	enum class TextureType : uint8_t
	{
		NONE = 0,
		DIFFUSE_TEXTURE,
		NORMAL_TEXTURE,
		SPECULAR_TEXTURE,
		OCCLUSION_TEXTURE
	};

	struct Image
	{
		int32_t width = 0;
		int32_t height = 0;
		int32_t channels = 0;
		TextureType type = TextureType::NONE;
		std::vector<unsigned char> pixels;
	};

	class TextureLayout
	{
	public:
		TextureLayout& add(const TParamName name, const TParamValue value)
		{
			m_layout.emplace_back(name, value);

			return *this;
		}

		[[nodiscard]] const std::vector<TextureParameter>& GetParameters() const { return m_layout; }
	private:
		std::vector<TextureParameter> m_layout;
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

		virtual void SetSubImage(DataType type, int32_t level, const void* pixels) const = 0;
		virtual void CopySubImage(int32_t level, int32_t width, int32_t height) const = 0;

		virtual void Bind(uint32_t unit = 0)   const = 0;
		virtual void Unbind(uint32_t unit = 0) const = 0;

		[[nodiscard]] virtual uint32_t GetID() const = 0;
	protected:
		int32_t p_width  = 0;
		int32_t p_height = 0;
	};
}