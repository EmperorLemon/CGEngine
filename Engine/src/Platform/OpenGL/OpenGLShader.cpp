#include "OpenGLShader.h"

#include "Core/Logger.hpp"

#include <glad/gl.h>

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

	static void FetchUniformNames(const uint32_t program, std::unordered_map<std::string, GLUniform>& uniforms)
	{
		GLint uniform_count = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);

		if (uniform_count != 0)
		{
			GLint	max_name_len = 0;
			GLsizei length = 0;
			GLsizei count  = 0;
			GLenum  type   = GL_NONE;
			glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

			const auto& uniform_name = std::make_unique<char[]>(max_name_len);

			for (GLint i = 0; i < uniform_count; ++i)
			{
				glGetActiveUniform(program, i, max_name_len, &length, &count, &type, uniform_name.get());

				GLUniform uniform = {};
				{
					uniform.location = glGetUniformLocation(program, uniform_name.get());
					uniform.count    = count;
				}

				uniforms.emplace(std::make_pair(std::string(uniform_name.get(), length), uniform));
			}
		}
	}

	GLShader::GLShader(ShaderModule modules[], const size_t count) : Shader(0)
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

		FetchUniformNames(p_id, m_uniforms);
	}

	GLShader::~GLShader()
	{
		//CG_TRACE("Deleted GLShader {0}", p_id);
		glDeleteProgram(p_id);
	}

	void GLShader::Use() const
	{
		glUseProgram(p_id);
	}

	void GLShader::Disable() const
	{
		glUseProgram(0);
	}

	void GLShader::BindUniform(const std::string_view name, const UniformType type, const void* value, const bool transpose) const
	{
		GLUniform uniform = {};

		if (!GetUniform(name.data(), uniform)) return;

		if (value == nullptr)
		{
			CG_WARN("value is nullptr!");
			return;
		}

		switch (type)
		{
		case UniformType::BOOL:			glProgramUniform1i(p_id, uniform.location, *static_cast<const GLboolean*>(value));							 break;
		case UniformType::INT:		    glProgramUniform1i(p_id, uniform.location, *static_cast<const GLint*>(value));								 break;
		case UniformType::UNSIGNED_INT: glProgramUniform1ui(p_id, uniform.location, *static_cast<const GLuint*>(value));						 break;
		case UniformType::FLOAT:		glProgramUniform1f(p_id, uniform.location, *static_cast<const GLfloat*>(value));								 break;
		case UniformType::VEC2:			glProgramUniform2fv(p_id, uniform.location, uniform.count, static_cast<const GLfloat*>(value));					 break;
		case UniformType::VEC3:			glProgramUniform3fv(p_id, uniform.location, uniform.count, static_cast<const GLfloat*>(value));					 break;
		case UniformType::VEC4:			glProgramUniform4fv(p_id, uniform.location, uniform.count, static_cast<const GLfloat*>(value));					 break;
		case UniformType::MAT3:			glProgramUniformMatrix3fv(p_id, uniform.location, uniform.count, transpose, static_cast<const GLfloat*>(value)); break;
		case UniformType::MAT4:			glProgramUniformMatrix4fv(p_id, uniform.location, uniform.count, transpose, static_cast<const GLfloat*>(value)); break;
		}
	}

	bool GLShader::GetUniform(const std::string& name, GLUniform& uniform) const
	{
		if (m_uniforms.contains(name))
		{
			uniform = m_uniforms.at(name);
			return true;
		}

		CG_WARN("Shader uniform {0} could not be found!", name.c_str());

		return false;
	}
}