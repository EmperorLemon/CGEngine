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
	struct Mesh;
}

namespace CGEngine::OpenGL
{
	class GLBuffer;
	class GLVertexArray;
	class GLTexture;

	class GLDrawObject
	{
	public:
		explicit GLDrawObject(Assets::Mesh&& mesh);

		Math::Vector3 position = Math::Vector3(0.0f);
		Math::Vector3 scale    = Math::Vector3(1.0f);

		[[nodiscard]] const GLVertexArray& GetVertexArray() const;
		[[nodiscard]]		Assets::Material& GetMaterial() const;

		[[nodiscard]] const std::vector<std::shared_ptr<GLTexture>>& GetTextures() const;
	private:
		std::shared_ptr<GLBuffer>	   m_vertexBuffer = nullptr;
		std::shared_ptr<GLVertexArray> m_vertexArray  = nullptr;

		std::shared_ptr<Assets::Material> m_material  = nullptr;

		std::vector<std::shared_ptr<GLTexture>> m_textures;
	};
}