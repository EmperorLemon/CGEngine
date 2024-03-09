#pragma once

#include <string_view>
#include <vector>

namespace CGEngine::Assets
{
	struct Mesh;
}

namespace CGEngine::IO
{
	bool LoadglTFModel(std::string_view filepath, std::vector<Assets::Mesh>& meshes);
}