#pragma once

#include "Renderer/RenderAPI.h"

namespace CGEngine::OpenGL
{
	class OpenGLAPI final : public RenderAPI
	{
	public:
		void Clear(uint32_t mask) override;
		void ClearColor(float* rgba) override;
	};
}
