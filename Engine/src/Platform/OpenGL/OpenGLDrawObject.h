#pragma once

#include <memory>
#include <vector>

#include "ECS/Component/DrawObject.h"

namespace CGEngine
{
	class VertexLayout;
}

namespace CGEngine::Assets
{
	struct Material;
	struct Model;
}

namespace CGEngine::OpenGL
{
	class GLBuffer;
	class GLVertexArray;
	class GLTexture;

	struct GLDrawObject : Component::DrawObject
	{
		explicit GLDrawObject(Assets::Model&& model);

		std::vector<GLBuffer>	   vertexBuffers;
		std::vector<GLVertexArray> vertexArrays;

		std::vector<Assets::Material> materials;
		std::vector<GLTexture>		  textures;
	};
}