#pragma once

#include "Types.h"
#include <string_view>

namespace CGEngine
{
	struct ShaderModule
	{
		std::string_view source;
		ShaderType type = ShaderType::NONE;
		uint32_t id = 0;
	};

	class Shader
	{
	public:
		explicit Shader(const uint32_t id) : p_id(id) {}

		Shader(Shader&&) noexcept = default;
		Shader(const Shader&) = delete;
		Shader& operator=(Shader&&) noexcept = default;
		Shader& operator=(const Shader&) = delete;

		virtual ~Shader() = default;

		virtual void Use()   const = 0;
		virtual void Disable() const = 0;

	protected:
		uint32_t p_id = 0;
	};
}