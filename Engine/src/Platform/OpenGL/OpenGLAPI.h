#pragma once

#include "Renderer/RenderAPI.h"

namespace CGEngine::OpenGL
{
	class OpenGLAPI final : public RenderAPI
	{
	public:
		void Draw(void* ptr) const override;

		void Enable(APICapability capability) const override;
		void Disable(APICapability capability) const override;

		void Clear(BufferMask mask) override;
		void ClearColor(const float* rgba) override;

		void ResizeViewport(int32_t x, int32_t y, int32_t width, int32_t height) override;
	};
}
