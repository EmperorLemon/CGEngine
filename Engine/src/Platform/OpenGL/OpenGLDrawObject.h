#pragma once

#include <memory>
#include <vector>

#include "Math/Vector3.h"

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

	class GLDrawObject
	{
	public:
		explicit GLDrawObject(Assets::Model&& model);

		Math::Vector3 position = Math::Vector3(0.0f);

		[[nodiscard]] Assets::Material& GetMaterial(size_t index);

		[[nodiscard]] const std::vector<GLVertexArray>& GetVertexArrays() const;
		[[nodiscard]] const std::vector<GLTexture>& GetTextures() const;
		[[nodiscard]] const std::vector<Assets::Material>& GetMaterials() const;
	private:
		std::vector<GLBuffer>	   m_vertexBuffers;
		std::vector<GLVertexArray> m_vertexArrays;

		std::vector<Assets::Material> m_materials;
		std::vector<GLTexture> m_textures;
	};
}