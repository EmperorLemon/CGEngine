#pragma once

#include <string_view>
#include <vector>

namespace CGEngine::Object
{
	struct Mesh;
}

namespace CGEngine::IO
{
	bool LoadglTFModel(std::string_view filepath, std::vector<Object::Mesh>& meshes);
}