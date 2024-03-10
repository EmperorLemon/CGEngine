#pragma once

#include <vector>

#include "Renderer/Vertex.h"
#include "Renderer/Texture.h"

namespace CGEngine::Assets
{
	struct Mesh
	{
		VertexLayout layout;
		std::vector<float> vertices;
		std::vector<uint16_t> indices;
	};
}
