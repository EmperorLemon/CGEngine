#pragma once

#include <vector>

#include "Material.h"

#include "Renderer/Vertex.h"
#include "Renderer/Texture.h"

namespace CGEngine::Assets
{
	struct Mesh
	{
		VertexLayout layout;
		std::vector<float> vertices;
		std::vector<uint16_t> indices;
		std::vector<Image> textures;
		Material material;
	};
}
