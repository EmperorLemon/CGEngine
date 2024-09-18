#pragma once

#include "Renderer/Shader.h"

#include <string>
#include <unordered_map>
#include <string_view>

namespace CGEngine::OpenGL
{
	struct GLUniform
	{
		int32_t location;
		int32_t count;
	};

	enum class UniformType : uint8_t
	{
		BOOL,
		UNSIGNED_INT,
		INT,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		MAT3,
		MAT4
	};

	class GLShader final : public Shader
	{
	public:
		explicit GLShader(ShaderModule modules[], size_t count);

		GLShader(GLShader&&) noexcept = delete;
		GLShader(const GLShader&) noexcept = delete;
		GLShader& operator=(GLShader&&) noexcept = delete;
		GLShader& operator=(const GLShader&) noexcept = delete;

		~GLShader() override;

		void Use() const override;
		void Disable() const override;

		void BindUniform(std::string_view name, UniformType type, const void* value, bool transpose = false) const;
	private:
		[[nodiscard]] bool GetUniform(const std::string& name, GLUniform& uniform) const;

		std::unordered_map<std::string, GLUniform> m_uniforms;
	};
}
