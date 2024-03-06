#pragma once

#include "Renderer/RenderAPI.h"

namespace CGEngine::OpenGL
{
	class OpenGLAPI final : public RenderAPI
	{
	public:
		void Draw(void* array_ptr) const override;

		void Enable(uint32_t capability) const override;
		void Disable(uint32_t capability) const override;

		void Clear(uint32_t mask) override;
		void ClearColor(float* rgba) override;
	};
}
