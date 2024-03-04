#pragma once

#include "Renderer/RenderAPI.h"

namespace CGEngine::OpenGL
{
	class OpenGLAPI final : public RenderAPI
	{
	public:
		void Draw(void* array_ptr) const override;
		void Clear(uint32_t mask) override;
		void ClearColor(float* rgba) override;
	};
}
