#pragma once

#include <vector>

namespace CGEngine::Object
{
	struct Mesh
	{
		std::vector<float>    vertices;
		std::vector<uint16_t> indices;
	};
}
