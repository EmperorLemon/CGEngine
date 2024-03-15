#include "DrawObject.h"

#include "Core/Logger.hpp"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include "Renderer/Assets/Model.h"

namespace CGEngine::Component
{
	DrawObject::DrawObject(Assets::Model&& model)
	{
		int index = 0;

		for (const auto& mesh : model.meshes)
		{
			const auto   vBufferSize = sizeof(float) * mesh.vertices.size();
			const auto   iBufferSize = sizeof(uint16_t) * mesh.indices.size();
			const size_t bufferSize = vBufferSize + iBufferSize;

			const BufferInfo vBufferInfo = { vBufferSize, mesh.vertices.size(), 0 };
			const BufferInfo iBufferInfo = { iBufferSize, mesh.indices.size(),  vBufferInfo.size };

			vertexBuffers.emplace_back(std::make_shared<OpenGL::GLBuffer>(BufferTarget::VERTEX_BUFFER, bufferSize, nullptr));
			vertexBuffers.at(index)->SetSubData(vBufferInfo.offset, vBufferInfo.size, mesh.vertices.data());
			vertexBuffers.at(index)->SetSubData(iBufferInfo.offset, iBufferInfo.size, mesh.indices.data());

			vertexArrays.emplace_back(std::make_shared<OpenGL::GLVertexArray>(vertexBuffers.at(index)->GetID(), vBufferInfo, &iBufferInfo, mesh.layout));
			vertexArrays.at(index)->SetDrawType(DrawType::DRAW_ELEMENTS);

			index++;
		}

		for (auto& texture : model.textures)
		{
			TextureLayout layout;

			layout.add(TParamName::TEXTURE_MIN_FILTER, TParamValue::LINEAR_MIPMAP_LINEAR);
			layout.add(TParamName::TEXTURE_MAG_FILTER, TParamValue::LINEAR);

			auto format = PixelFormat::NONE;

			switch (texture.channels)
			{
			case 0: CG_WARN("Texture Channels = 0");  break;
			case 1: format = PixelFormat::R8;         break;
			case 2: format = PixelFormat::RG8;		  break;
			case 3: format = PixelFormat::RGB8;		  break;
			case 4: format = PixelFormat::RGBA8;	  break;
			default:								  break;
			}

			textures.emplace_back(std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, format, texture.width, texture.height, layout, texture.pixels.data()));

			std::vector<unsigned char> emptyPixels;
			texture.pixels.swap(emptyPixels);
		}

		materials.swap(model.materials);

		std::vector<Assets::Mesh> emptyMeshes;
		std::vector<Image>		  emptyTextures;

		model.meshes.swap(emptyMeshes);
		model.textures.swap(emptyTextures);
	}
}
