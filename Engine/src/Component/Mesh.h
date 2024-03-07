#pragma once

#include <vector>

#include "Renderer/Vertex.h"

namespace CGEngine::Object
{

	struct Mesh
	{
		VertexLayout layout;
		std::vector<float> vertices;
		std::vector<uint16_t> indices;
	};
}
