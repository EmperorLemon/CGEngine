#include "OpenGLDrawObject.h"

#include "Core/Logger.hpp"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Renderer/Assets/Mesh.h"

namespace CGEngine::OpenGL
{
	GLDrawObject::GLDrawObject(Assets::Mesh&& mesh) : m_material(std::make_shared<Assets::Material>(mesh.material))
	{
		const auto  vBufferSize = sizeof(float) * mesh.vertices.size();
		const auto  iBufferSize = sizeof(uint16_t) * mesh.indices.size();
		const size_t bufferSize = vBufferSize + iBufferSize;

		const BufferInfo vBufferInfo = { vBufferSize, mesh.vertices.size(), 0 };
		const BufferInfo iBufferInfo = { iBufferSize, mesh.indices.size(),  vBufferInfo.size};

		m_vertexBuffer = std::make_shared<GLBuffer>(BufferTarget::VERTEX_BUFFER, bufferSize, nullptr);
		m_vertexBuffer->SetSubData(vBufferInfo.offset, vBufferInfo.size, mesh.vertices.data());
		m_vertexBuffer->SetSubData(iBufferInfo.offset, iBufferInfo.size, mesh.indices.data());

		m_vertexArray = std::make_shared<GLVertexArray>(m_vertexBuffer->GetID(), vBufferInfo, &iBufferInfo, mesh.layout);

		std::vector<unsigned char> empty;

		for (auto& texture : mesh.textures)
		{
			m_textures.emplace_back(std::make_shared<GLTexture>(TextureTarget::TEXTURE_2D, PixelFormat::RGBA8, texture.width, texture.height, texture.pixels.data()));

			texture.pixels.clear();
			texture.pixels.swap(empty);
		}
	}

	const GLVertexArray& GLDrawObject::GetVertexArray() const
	{
		return *m_vertexArray;
	}

	Assets::Material& GLDrawObject::GetMaterial() const
	{
		return *m_material;
	}

	const std::vector<std::shared_ptr<GLTexture>>& GLDrawObject::GetTextures() const
	{
		return m_textures;
	}
}
