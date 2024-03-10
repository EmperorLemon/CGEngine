#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Light.h"

namespace CGEngine::Assets
{
	struct Model
	{
		std::vector<Mesh>  meshes;
		std::vector<Material> materials;
		std::vector<Light> lights;
		std::vector<Image> textures;
	};
}
