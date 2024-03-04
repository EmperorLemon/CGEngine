#include "OpenGLShader.h"

#include "Core/Logger.hpp"

#include <glad/glad.h>

namespace CGEngine::OpenGL
{
	constexpr void CheckCompileErrors(const GLuint shader, const ShaderType type)
	{
		GLint  success;
		GLchar infoLog[1024];

		const auto shader_type = [type]
		{
			switch (type)
			{
			case ShaderType::NONE: break;
			case ShaderType::VERTEX: return "VERTEX SHADER";
			case ShaderType::GEOMETRY: return "GEOMETRY SHADER";
			case ShaderType::TESSELATION_CONTROL: return "TESSELATION CONTROL SHADER";
			case ShaderType::TESSELATION_EVALUATION: return "TESSELATION EVALUATION SHADER";
			case ShaderType::FRAGMENT: return "FRAGMENT SHADER";
			case ShaderType::COMPUTE: return "COMPUTE SHADER";
			case ShaderType::PROGRAM: return "SHADER PROGRAM";
			}

			return "UNKNOWN SHADER";
		}();

		if (type != ShaderType::PROGRAM)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				CG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}", shader_type, infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);

			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				CG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {0}\n{1}", shader_type, infoLog);
			}
		}
	}

	constexpr bool is_vertex(const ShaderModule& module) { return module.type == ShaderType::VERTEX; }
	constexpr bool is_fragment(const ShaderModule& module) { return module.type == ShaderType::FRAGMENT; }

	GLShader::GLShader(ShaderModule modules[], const int8_t count) : Shader(0)
	{
		if (count < 2) return;

		const auto& vertex_module = std::find_if(modules, modules + count, is_vertex);

		if (vertex_module != nullptr)
		{
			vertex_module->id = glCreateShader(GL_VERTEX_SHADER);
			const char* vertex_src = vertex_module->source.data();

			glShaderSource(vertex_module->id, 1, &vertex_src, nullptr);
			glCompileShader(vertex_module->id);
			CheckCompileErrors(vertex_module->id, vertex_module->type);
		}
		else
			CG_ERROR("The vertex shader is nullptr!");

		const auto& fragment_module = std::find_if(modules, modules + count, is_fragment);

		if (fragment_module)
		{
			fragment_module->id = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fragment_src = fragment_module->source.data();

			glShaderSource(fragment_module->id, 1, &fragment_src, nullptr);
			glCompileShader(fragment_module->id);
			CheckCompileErrors(fragment_module->id, fragment_module->type);
		}

		p_id = glCreateProgram();
		glAttachShader(p_id, vertex_module->id);
		glAttachShader(p_id, fragment_module->id);
		glLinkProgram(p_id);
		CheckCompileErrors(p_id, ShaderType::PROGRAM);

		glDeleteShader(vertex_module->id);
		glDeleteShader(fragment_module->id);
	}

	void GLShader::Bind() const
	{
		glUseProgram(p_id);
	}

	void GLShader::Unbind() const
	{
		glUseProgram(0);
	}
}