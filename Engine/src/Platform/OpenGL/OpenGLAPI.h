#pragma once

#include "Renderer/RenderAPI.h"

namespace CGEngine::OpenGL
{
	class OpenGLAPI final : public RenderAPI
	{
	public:
		void Draw(void* ptr) const override;

		void Enable(uint32_t capability) const override;
		void Disable(uint32_t capability) const override;

		void Clear(uint32_t mask) override;
		void ClearColor(float* rgba) override;

		void ResizeViewport(int32_t x, int32_t y, int32_t width, int32_t height) override;
	};
}
