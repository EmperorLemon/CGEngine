#pragma once

#include "Renderer/RenderAPI.h"

namespace CGEngine
{
	class VertexArray;
}

namespace CGEngine::OpenGL
{
	class OpenGLAPI final : public RenderAPI
	{
	public:
		void BindTexture(uint32_t texture, int32_t unit) override;

		void Clear(BufferMask mask) override;
		void ClearColor(const float* rgba) override;
		void ClearColor(float r, float g, float b, float a) override;

		void CullFace(CGEngine::CullFace face) const override;

		void Draw(const VertexArray* vertexArrayPtr) const override;

		void Enable(APICapability capability) const override;
		void Disable(APICapability capability) const override;

		void ResizeViewport(int32_t x, int32_t y, int32_t width, int32_t height) override;

		void SetDepthFunc(DepthFunc func) const override;
		void SetDrawMode(PolygonMode mode) const override;
	};
}
