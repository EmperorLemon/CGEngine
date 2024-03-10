#pragma once

#include <string_view>

namespace CGEngine::Assets
{
	struct Model;
}

namespace CGEngine::IO
{
	void AssimpLoadModel(std::string_view filepath, Assets::Model& model);
}