#include "glTFLoader.h"

#include "Renderer/Assets/Model.h"
#include "Math/Math.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

#include <stack>

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
		size_t count  = 0;
		int32_t stride = 0;

		bool HAS_POSITIONS = false;
		bool HAS_NORMALS   = false;
		bool HAS_TEXCOORDS = false;
		//bool HAS_TANGENTS  = false;

		for (const auto& primitive : gltfMesh.primitives)
		{
			if (primitive.indices >= 0)
			{
				const auto& accessor   = model.accessors.at(primitive.indices);
				const auto& bufferView = model.bufferViews.at(accessor.bufferView);
				const auto& buffer	   = model.buffers.at(bufferView.buffer);
				const auto  indexData  = reinterpret_cast<const uint16_t*>(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);
					
				mesh.indices.insert(mesh.indices.end(), indexData, indexData + accessor.count);
			}

			std::vector<Math::Vec3> positions;

			if (primitive.attributes.contains("POSITION"))
			{
				HAS_POSITIONS = true;

				const auto& accessor = model.accessors[primitive.attributes.at("POSITION")];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer = model.buffers[bufferView.buffer];
				const auto  byteOffset = bufferView.byteOffset + accessor.byteOffset;

				count = accessor.count;
				stride += accessor.ByteStride(bufferView);

				for (size_t i = 0; i < accessor.count; ++i)
				{
					const auto data = reinterpret_cast<const float*>(buffer.data.data() + byteOffset + i * accessor.ByteStride(bufferView));

					positions.emplace_back(Math::ToVec3(data));
				}
			}

			std::vector<Math::Vec3> normals;

			if (primitive.attributes.contains("NORMAL"))
			{
				HAS_NORMALS = true;

				const auto& accessor = model.accessors[primitive.attributes.at("NORMAL")];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer = model.buffers[bufferView.buffer];
				const auto  byteOffset = bufferView.byteOffset + accessor.byteOffset;

				stride += accessor.ByteStride(bufferView);

				for (size_t i = 0; i < accessor.count; ++i)
				{
					const auto data = reinterpret_cast<const float*>(buffer.data.data() + byteOffset + i * accessor.ByteStride(bufferView));

					normals.emplace_back(Math::ToVec3(data));
				}
			}

			std::vector<Math::Vec2> uvs;

			if (primitive.attributes.contains("TEXCOORD_0"))
			{
				HAS_TEXCOORDS = true;

				const auto& accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer = model.buffers[bufferView.buffer];
				const auto  byteOffset = bufferView.byteOffset + accessor.byteOffset;

				stride += accessor.ByteStride(bufferView);

				for (size_t i = 0; i < accessor.count; ++i)
				{
					const auto data = reinterpret_cast<const float*>(buffer.data.data() + byteOffset + i * accessor.ByteStride(bufferView));

					uvs.emplace_back(Math::ToVec2(data));
				}
			}

			if (HAS_POSITIONS)
				mesh.layout.add(0, 3, DataType::FLOAT, 0, false);
			if (HAS_NORMALS)
				mesh.layout.add(1, 3, DataType::FLOAT, 3 * sizeof(float), false);
			if (HAS_TEXCOORDS)
				mesh.layout.add(2, 2, DataType::FLOAT, 6 * sizeof(float), false);

			mesh.layout.SetStride(stride);

			for (size_t i = 0; i < count; ++i)
			{
				if (!positions.empty() && HAS_POSITIONS)
				{
					const auto& position = positions.at(i);

					mesh.vertices.emplace_back(position.x);
					mesh.vertices.emplace_back(position.y);
					mesh.vertices.emplace_back(position.z);
				}

				if (!normals.empty() && HAS_NORMALS)
				{
					const auto& normal = normals.at(i);

					mesh.vertices.emplace_back(normal.x);
					mesh.vertices.emplace_back(normal.y);
					mesh.vertices.emplace_back(normal.z);
				}

				if (!uvs.empty() && HAS_TEXCOORDS)
				{
					const auto& uv = uvs.at(i);

					mesh.vertices.emplace_back(uv.s);
					mesh.vertices.emplace_back(uv.t);
				}
			}
		}
	}

	static void ExtractMaterialsAndTextures(const tinygltf::Model& importModel, const tinygltf::Mesh& gltfMesh, Assets::Model& model)
	{
		for (const auto& primitive : gltfMesh.primitives)
		{
			if (primitive.material >= 0 && primitive.material < static_cast<int32_t>(importModel.materials.size()))
			{
				const auto& gltfMaterial = importModel.materials.at(primitive.material);

				const auto& baseColorFactor = gltfMaterial.pbrMetallicRoughness.baseColorFactor;
				const auto& baseColor = Math::Vec4(baseColorFactor.at(0), baseColorFactor.at(1), baseColorFactor.at(2), baseColorFactor.at(3));
				const auto  metallicFactor  = static_cast<float>(gltfMaterial.pbrMetallicRoughness.metallicFactor);
				const auto  roughnessFactor = static_cast<float>(gltfMaterial.pbrMetallicRoughness.roughnessFactor);

				model.materials.emplace_back(baseColor, metallicFactor, roughnessFactor);

				if (!importModel.textures.empty())
				{
					if (gltfMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
					{
						const auto& texture = importModel.textures.at(gltfMaterial.pbrMetallicRoughness.baseColorTexture.index);
								auto& image = importModel.images.at(texture.source);

						model.textures.emplace_back(image.width, image.height, 4, TextureType::DIFFUSE_TEXTURE, image.image);
					}

					if (gltfMaterial.normalTexture.index >= 0)
					{
						const auto& texture = importModel.textures.at(gltfMaterial.normalTexture.index);
								auto& image = importModel.images.at(texture.source);

						model.textures.emplace_back(image.width, image.height, 4, TextureType::NORMAL_TEXTURE, image.image);
					}

					if (gltfMaterial.occlusionTexture.index >= 0)
					{
						const auto& texture = importModel.textures.at(gltfMaterial.occlusionTexture.index);
						auto& image = importModel.images.at(texture.source);

						model.textures.emplace_back(image.width, image.height, 3, TextureType::OCCLUSION_TEXTURE, image.image);
					}

					//if (gltfMaterial.emissiveTexture.index >= 0)
					//{
					//	const auto& texture = importModel.textures.at(gltfMaterial.emissiveTexture.index);
					//	auto& image = importModel.images.at(texture.source);

					//	model.textures.emplace_back(image.width, image.height, 4, image.image);
					//}
				}
			}
		}
	}

	static void ProcessModel(const tinygltf::Model& importModel, const tinygltf::Node& node, Assets::Model& model)
	{
		Assets::Mesh	 mesh = {};

		if (node.mesh < static_cast<int>(importModel.meshes.size()))
		{
			const auto& gltfMesh = importModel.meshes.at(node.mesh);

			ProcessMesh(importModel, gltfMesh, mesh);
			model.meshes.push_back(std::move(mesh));

			if (!importModel.materials.empty())
			{
				ExtractMaterialsAndTextures(importModel, gltfMesh, model);
			}
		}
	}

	static void ExtractModel(const tinygltf::Model& importModel, Assets::Model& model)
	{
		const auto& scene = importModel.scenes.at(importModel.defaultScene);

		for (const auto node : scene.nodes)
		{
			ProcessModel(importModel, importModel.nodes.at(node), model);
		}
	}

	bool LoadglTFModel(const std::string_view filepath, Assets::Model& model)
	{
		tinygltf::TinyGLTF loader;
		tinygltf::Model importModel;

		std::string error, warn;

		const bool success = loader.LoadASCIIFromFile(&importModel, &error, &warn, filepath.data());

		if (!error.empty())
			CG_ERROR("glTF Error: {0}", error.c_str());

		if (!warn.empty())
			CG_WARN("glTF Warning: {0}", warn.c_str());

		if (!success)
			CG_ERROR("Failed to load glTF: {0}", filepath.data());
		else
			CG_INFO("Loaded glTF: {0}", filepath.data());

		ExtractModel(importModel, model);

		return success;
	}
}