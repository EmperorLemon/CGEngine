#include "glTFLoader.h"

#include "Component/Mesh.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include <tiny_gltf.h>

#include <stack>
#include <algorithm>

#include "Core/Logger.hpp"

namespace CGEngine::IO
{
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

	//static void ProcessBuffer()

	static void ProcessMesh(const tinygltf::Model& model, const tinygltf::Mesh& gltfMesh, Object::Mesh& mesh)
	{
		for (const auto& primitive : gltfMesh.primitives)
		{
			{
				const auto& accessor   = model.accessors[primitive.indices];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer	   = model.buffers[bufferView.buffer];
				const auto  indexData  = reinterpret_cast<const uint16_t*>(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);

				mesh.indices.insert(mesh.indices.end(), indexData, indexData + accessor.count);
			}

			if (primitive.attributes.contains("POSITION"))
			{
				const auto& accessor   = model.accessors[primitive.attributes.at("POSITION")];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer     = model.buffers[bufferView.buffer];
				const auto  byteOffset = bufferView.byteOffset + accessor.byteOffset;

				mesh.layout.add(0, accessor.type, DataType::FLOAT, byteOffset, accessor.normalized);

				for (size_t i = 0; i < accessor.count; ++i)
				{
					const auto data = reinterpret_cast<const float*>(buffer.data.data() + byteOffset + i * accessor.ByteStride(bufferView));
					mesh.vertices.insert(mesh.vertices.end(), data, data + accessor.type);
				}
			}

			if (primitive.attributes.contains("NORMAL"))
			{
				const auto& accessor = model.accessors[primitive.attributes.at("NORMAL")];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer = model.buffers[bufferView.buffer];
				const auto  byteOffset = bufferView.byteOffset + accessor.byteOffset;

				mesh.layout.add(1, accessor.type, DataType::FLOAT, byteOffset, accessor.normalized);

				for (size_t i = 0; i < accessor.count; ++i)
				{
					const auto data = reinterpret_cast<const float*>(buffer.data.data() + byteOffset + i * accessor.ByteStride(bufferView));
					mesh.vertices.insert(mesh.vertices.end(), data, data + accessor.type);
				}
			}

			if (primitive.attributes.contains("TEXCOORD_0"))
			{
				const auto& accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer = model.buffers[bufferView.buffer];
				const auto  byteOffset = bufferView.byteOffset + accessor.byteOffset;

				mesh.layout.add(2, accessor.type, DataType::FLOAT, byteOffset, accessor.normalized);

				for (size_t i = 0; i < accessor.count; ++i)
				{
					const auto data = reinterpret_cast<const float*>(buffer.data.data() + byteOffset + i * accessor.ByteStride(bufferView));
					mesh.vertices.insert(mesh.vertices.end(), data, data + 2);
				}
			}

			mesh.layout.SetStride(3 * sizeof(float));
		}
	}

	static void ExtractMeshes(const tinygltf::Model& model, const tinygltf::Node& parentNode, std::vector<Object::Mesh>& meshes)
	{
		std::stack<tinygltf::Node> nodeStack;
		nodeStack.push(parentNode);

		while (!nodeStack.empty())
		{
			const auto& currentNode = nodeStack.top();
			nodeStack.pop();

			if (currentNode.mesh >= 0 && currentNode.mesh < static_cast<int32_t>(model.meshes.size()))
			{
				Object::Mesh mesh = {};
				ProcessMesh(model, model.meshes[currentNode.mesh], mesh);

				meshes.push_back(mesh);
			}

			for (const int node : currentNode.children)
				nodeStack.push(model.nodes[node]);
		}
	}

	static void SubmitModel(const tinygltf::Model& model, std::vector<Object::Mesh>& meshes)
	{
		const auto& scene = model.scenes[model.defaultScene];

		for (const int node : scene.nodes)
		{
			ExtractMeshes(model, model.nodes[node], meshes);
		}
	}

	bool LoadglTFModel(const std::string_view filepath, std::vector<Object::Mesh>& meshes)
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