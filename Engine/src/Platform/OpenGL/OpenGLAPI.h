#pragma once

#include "Renderer/RenderAPI.h"

namespace CGEngine::OpenGL
{
	struct OpenGLAPI : RenderAPI
	{
		void Clear(uint32_t mask) override;
		void ClearColor(float* rgba) override;
	};
}
