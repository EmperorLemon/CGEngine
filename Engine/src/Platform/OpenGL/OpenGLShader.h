#pragma once

#include "Renderer/Shader.h"

namespace CGEngine::OpenGL
{
	class GLShader final : public Shader
	{
	public:
		explicit GLShader(ShaderModule modules[], int8_t count);

		void Bind() const override;
		void Unbind() const override;
	};
}
