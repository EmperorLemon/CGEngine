#pragma once

#include "Renderer/Shader.h"

#include <unordered_map>
#include <string_view>

namespace CGEngine::OpenGL
{
	struct GLUniform
	{
		int32_t location;
		int32_t count;
	};

	class GLShader final : public Shader
	{
	public:
		explicit GLShader(ShaderModule modules[], size_t count);

		GLShader(GLShader&&) noexcept = default;
		GLShader& operator=(GLShader&&) noexcept = default;
		GLShader(const GLShader&) = delete;
		GLShader& operator=(const GLShader&) = delete;

		~GLShader() override;

		void Bind() const override;
		void Unbind() const override;

		[[nodiscard]] const GLUniform& GetUniform(std::string_view name) const;
	private:
		std::unordered_map<std::string_view, GLUniform> m_uniforms;
	};
}
