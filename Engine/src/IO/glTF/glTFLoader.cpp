#include "glTFLoader.h"

#include "Component/Mesh.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

#include <stack>

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

	static void ProcessMesh(const tinygltf::Model& model, const tinygltf::Mesh& gltfMesh, Object::Mesh& mesh)
	{
		for (const auto& primitive : gltfMesh.primitives)
		{
			const auto& indexAccessor = model.accessors[primitive.indices];
			const auto& indexView     = model.bufferViews[indexAccessor.bufferView];
			const auto& indexBuffer   = model.buffers[indexView.buffer];
			const auto  indexData     = reinterpret_cast<const uint16_t*>(indexBuffer.data.data() + indexView.byteOffset + indexAccessor.byteOffset);

			mesh.indices.insert(mesh.indices.end(), indexData, indexData + indexAccessor.count);

			std::vector<float> vertices;

			for (const auto& attribute : primitive.attributes)
			{
				const auto& accessor = model.accessors[attribute.second];
				const auto& bufferView = model.bufferViews[accessor.bufferView];
				const auto& buffer = model.buffers[bufferView.buffer];
				const auto  vertexData = reinterpret_cast<const float*>(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);

				const auto attributeOffset = bufferView.byteOffset + accessor.byteOffset;

				for (size_t i = 0; i < accessor.count; ++i)
				{
					
				}
			}
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

	//static void CheckModel(const tinygltf::Model& model, Object::Mesh& mesh)
	//{
	//	const auto& gltfMesh = model.meshes[0];
	//	const auto& primitive = gltfMesh.primitives[0];

	//	const auto& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
	//	const auto& posView = model.bufferViews[posAccessor.bufferView];
	//	const auto& posBuffer = model.buffers[posView.buffer];
	//	const auto  posData = reinterpret_cast<const float*>(posBuffer.data.data() + posView.byteOffset + posAccessor.byteOffset);

	//	const auto& indexAccessor = model.accessors[primitive.indices];
	//	const auto& indexView     = model.bufferViews[indexAccessor.bufferView];
	//	const auto& indexBuffer   = model.buffers[indexView.buffer];
	//	const auto  indexData = reinterpret_cast<const uint16_t*>(indexBuffer.data.data() + indexView.byteOffset + indexAccessor.byteOffset);

	//	const int32_t nComponents = posAccessor.componentType == TINYGLTF_TYPE_SCALAR ? 1 : posAccessor.componentType == TINYGLTF_TYPE_VEC2 ? 2 : 3;

	//	mesh.vertices.reserve(posAccessor.count * nComponents);
	//	mesh.indices.reserve(indexAccessor.count);

	//	const size_t v_size = posAccessor.count * nComponents;
	//	const size_t i_size = indexAccessor.count;

	//	for (size_t i = 0; i < v_size; ++i)
	//		mesh.vertices.push_back(posData[i]);

	//	for (size_t i = 0; i < i_size; ++i)
	//		mesh.indices.push_back(indexData[i]);
	//}

	bool LoadglTFModel(const std::string_view filepath, std::vector<Object::Mesh>& meshes)
	{
		tinygltf::TinyGLTF loader;
		tinygltf::Model model;

		std::string error, warn;

		const bool success = loader.LoadASCIIFromFile(&model, &error, &warn, filepath.data());

		if (!error.empty())
			CG_ERROR("glTF Error: {0}", error.c_str());

		if (!warn.empty())
			CG_ERROR("glTF Warning: {0}", warn.c_str());

		if (!success)
			CG_ERROR("Failed to load glTF: {0}", filepath.data());
		else
			CG_INFO("Loaded glTF: {0}", filepath.data());

		SubmitModel(model, meshes);
		//CheckModel(model, meshes);

		return success;
	}

}