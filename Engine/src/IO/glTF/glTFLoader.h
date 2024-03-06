#pragma once

#include <string_view>

namespace CGEngine::Object
{
	struct Mesh;
}

namespace CGEngine::IO
{
	bool LoadglTFModel(std::string_view filepath, Object::Mesh& mesh);
}