#pragma once

#include <vector>
#include <memory>

namespace CGEngine::Assets
{
	struct Material;
	struct Model;
}

namespace CGEngine
{
	class Buffer;
	class VertexArray;
	class Texture;
}

namespace CGEngine::Component
{
	struct DrawObject
	{
		explicit DrawObject(Assets::Model&& model);

		std::vector<std::shared_ptr<Buffer>>	  vertexBuffers;
		std::vector<std::shared_ptr<VertexArray>> vertexArrays;

		std::vector<Assets::Material> materials;
		std::vector<std::shared_ptr<Texture>>	  textures;
	};
}