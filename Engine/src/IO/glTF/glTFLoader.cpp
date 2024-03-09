#include "glTFLoader.h"

#include "Renderer/Assets/Mesh.h"

#include "Math/Math.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

#include <stack>
#include <algorithm>

#include "Core/Logger.hpp"

namespace CGEngine::IO
{
	constexpr DataType Convert(const int32_t type)
	{
		switch (type)
		{
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:  return DataType::UNSIGNED_BYTE;
		case TINYGLTF_COMPONENT_TYPE_BYTE:			 return DataType::BYTE;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: return DataType::UNSIGNED_SHORT;
		case TINYGLTF_COMPONENT_TYPE_SHORT:			 return DataType::SHORT;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:	 return DataType::UNSIGNED_INT;
		case TINYGLTF_COMPONENT_TYPE_INT:			 return DataType::INT;
		case TINYGLTF_COMPONENT_TYPE_FLOAT:			 return DataType::FLOAT;
		case TINYGLTF_COMPONENT_TYPE_DOUBLE:		 return DataType::DOUBLE;
		default: return DataType::FLOAT; // Unknown component type
		}
	}

	constexpr size_t GetSizeOfComponentType(const int32_t type)
	{
		switch (type)
		{
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
		case TINYGLTF_COMPONENT_TYPE_BYTE:   return sizeof(int8_t);
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
		case TINYGLTF_COMPONENT_TYPE_SHORT:  return sizeof(int16_t);
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
		case TINYGLTF_COMPONENT_TYPE_INT:    return sizeof(int32_t);
		case TINYGLTF_COMPONENT_TYPE_FLOAT:  return sizeof(float);
		case TINYGLTF_COMPONENT_TYPE_DOUBLE: return sizeof(double);
		default: return 0; // Unknown component type
		}
	}

	static void ProcessMesh(const tinygltf::Model& model, const tinygltf::Mesh& gltfMesh, Assets::Mesh& mesh)
	{
		mesh.layout.SetStride(3 * sizeof(float));

		mesh.layout.add(0, 3, DataType::FLOAT,   0, false);
		mesh.layout.add(1, 2, DataType::FLOAT, 3 * sizeof(float), false);
		//mesh.layout.add(2, 2, DataType::FLOAT, 576, false);

		for (const auto& primitive : gltfMesh.primitives)
		{
			{
				const auto& accessor   = model.accessors.at(primitive.indices);
				const auto& bufferView = model.bufferViews.at(accessor.bufferView);
				const auto& buffer	   = model.buffers.at(bufferView.buffer);
				const auto  indexData  = reinterpret_cast<const uint16_t*>(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);

				mesh.indices.insert(mesh.indices.end(), indexData, indexData + accessor.count);
			}

			std::vector<Math::Vector3> positions;

			if (primitive.attributes.contains("POSITION"))
			{
				const auto& accessor = model.accessors[primitive.attributes.at("POSITION")];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer = model.buffers[bufferView.buffer];
				const auto  byteOffset = bufferView.byteOffset + accessor.byteOffset;

				for (size_t i = 0; i < accessor.count; ++i)
				{
					const auto data = reinterpret_cast<const float*>(buffer.data.data() + byteOffset + i * accessor.ByteStride(bufferView));

					positions.emplace_back(Math::make_vec3(data));
				}
			}

			std::vector<Math::Vector2> uvs;

			if (primitive.attributes.contains("TEXCOORD_0"))
			{
				const auto& accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer = model.buffers[bufferView.buffer];
				const auto  byteOffset = bufferView.byteOffset + accessor.byteOffset;

				for (size_t i = 0; i < accessor.count; ++i)
				{
					const auto data = reinterpret_cast<const float*>(buffer.data.data() + byteOffset + i * accessor.ByteStride(bufferView));

					uvs.emplace_back(Math::make_vec2(data));
				}
			}

			if (!model.materials.empty() && primitive.material >= 0 && primitive.material < static_cast<int32_t>(model.materials.size()))
			{
				const auto& material = model.materials.at(primitive.material);

				if (!material.pbrMetallicRoughness.baseColorFactor.empty())
				{
					const auto& baseColorFactor= material.pbrMetallicRoughness.baseColorFactor;
					mesh.material.baseColor = Math::Vector4(baseColorFactor.at(0), baseColorFactor.at(1), baseColorFactor.at(2), baseColorFactor.at(3));

					if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
					{
						const auto& texture = model.textures.at(material.pbrMetallicRoughness.baseColorTexture.index);
						auto& gltfImage	= model.images.at(texture.source);

						mesh.textures.emplace_back(gltfImage.width, gltfImage.width, 4, gltfImage.image);
					}
				}

				if (!material.doubleSided)
				{
					
				}
			}

			for (const auto& position : positions)
			{
				mesh.vertices.emplace_back(position.x);
				mesh.vertices.emplace_back(position.y);
				mesh.vertices.emplace_back(position.z);
			}

			for (const auto& uv : uvs)
			{
				mesh.vertices.emplace_back(uv.s);
				mesh.vertices.emplace_back(uv.t);
			}
		}
	}

	static void ExtractMeshes(const tinygltf::Model& model, const tinygltf::Node& parentNode, std::vector<Assets::Mesh>& meshes)
	{
		Assets::Mesh mesh = {};

		if (parentNode.mesh < static_cast<int>(model.meshes.size()))
		{
			ProcessMesh(model, model.meshes.at(parentNode.mesh), mesh);
			meshes.push_back(mesh);
		}
	}

	static void SubmitModel(const tinygltf::Model& model, std::vector<Assets::Mesh>& meshes)
	{
		const auto& scene = model.scenes.at(model.defaultScene);

		ExtractMeshes(model, model.nodes.at(0), meshes);
	}

	bool LoadglTFModel(const std::string_view filepath, std::vector<Assets::Mesh>& meshes)
	{
		tinygltf::TinyGLTF loader;
		tinygltf::Model model;

		std::string error, warn;

		const bool success = loader.LoadASCIIFromFile(&model, &error, &warn, filepath.data());

		if (!error.empty())
			CG_ERROR("glTF Error: {0}", error.c_str());

		if (!warn.empty())
			CG_WARN("glTF Warning: {0}", warn.c_str());

		if (!success)
			CG_ERROR("Failed to load glTF: {0}", filepath.data());
		else
			CG_INFO("Loaded glTF: {0}", filepath.data());

		SubmitModel(model, meshes);

		return success;
	}

}