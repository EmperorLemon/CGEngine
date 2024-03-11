#include "OpenGLDrawObject.h"

#include "Core/Logger.hpp"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Renderer/Assets/Model.h"

namespace CGEngine::OpenGL
{
	GLDrawObject::GLDrawObject(Assets::Model&& model)
	{
		int index = 0;

		for (const auto& mesh : model.meshes)
		{
			const auto   vBufferSize = sizeof(float) * mesh.vertices.size();
			const auto   iBufferSize = sizeof(uint16_t) * mesh.indices.size();
			const size_t bufferSize = vBufferSize + iBufferSize;

			const BufferInfo vBufferInfo = { vBufferSize, mesh.vertices.size(), 0 };
			const BufferInfo iBufferInfo = { iBufferSize, mesh.indices.size(),  vBufferInfo.size };

			m_vertexBuffers.emplace_back(BufferTarget::VERTEX_BUFFER, bufferSize, nullptr);
			m_vertexBuffers.at(index).SetSubData(vBufferInfo.offset, vBufferInfo.size, mesh.vertices.data());
			m_vertexBuffers.at(index).SetSubData(iBufferInfo.offset, iBufferInfo.size, mesh.indices.data());

			m_vertexArrays.emplace_back(m_vertexBuffers.at(index).GetID(), vBufferInfo, &iBufferInfo, mesh.layout);
			m_vertexArrays.at(index).SetDrawType(DrawType::DRAW_ELEMENTS);
			index++;
		}

		for (auto& texture : model.textures)
		{
			m_textures.emplace_back(TextureTarget::TEXTURE_2D, PixelFormat::RGBA8, texture.width, texture.height, texture.pixels.data());

			std::vector<unsigned char> pixels;
			texture.pixels.swap(pixels);
		}

		m_materials.swap(model.materials);

		std::vector<Assets::Mesh> meshes;
		std::vector<Image>		  textures;
		model.meshes.swap(meshes);
		model.textures.swap(textures);
	}

	const std::vector<GLVertexArray>& GLDrawObject::GetVertexArrays() const
	{
		return m_vertexArrays;
	}

	Assets::Material& GLDrawObject::GetMaterial(const size_t index)
	{
		return m_materials.at(index);
	}

	const std::vector<Assets::Material>& GLDrawObject::GetMaterials() const
	{
		return m_materials;
	}

	const std::vector<GLTexture>& GLDrawObject::GetTextures() const
	{
		return m_textures;
	}
}
