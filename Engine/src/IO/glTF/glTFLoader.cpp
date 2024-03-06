#include "glTFLoader.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include <stack>

#include "Core/Logger.hpp"
#include "Component/Mesh.h"

namespace CGEngine::IO
{
	static void ProcessMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
	{
		for (const auto& bufferView : model.bufferViews)
		{
			if (bufferView.target == 0)
			{
				CG_WARN("glTF Warning: bufferView target is 0");
				continue;
			}
		}

		for (const auto& primitive : mesh.primitives)
		{
			tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

			for (const auto& attribute : primitive.attributes)
			{
				const tinygltf::Accessor accessor = model.accessors[attribute.second];
			}
		}
	}

	static void ExtractMeshes(const tinygltf::Model& model, const tinygltf::Node& parentNode)
	{
		std::stack<tinygltf::Node> nodeStack;
		nodeStack.push(parentNode);

		while (!nodeStack.empty())
		{
			const auto& currentNode = nodeStack.top();
			nodeStack.pop();

			if (currentNode.mesh >= 0 && currentNode.mesh < static_cast<int32_t>(model.meshes.size()))
				ProcessMesh(model, model.meshes[currentNode.mesh]);

			for (const int node : currentNode.children)
				nodeStack.push(model.nodes[node]);
		}
	}

	static void SubmitModel(const tinygltf::Model& model)
	{
		const auto& scene = model.scenes[model.defaultScene];

		for (const int node : scene.nodes)
		{
			ExtractMeshes(model, model.nodes[node]);
		}
	}

	static void CheckModel(const tinygltf::Model& model, Object::Mesh& mesh)
	{
		const auto& gltfMesh = model.meshes[0];
		const auto& primitive = gltfMesh.primitives[0];

		const auto& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
		const auto& posView = model.bufferViews[posAccessor.bufferView];
		const auto& posBuffer = model.buffers[posView.buffer];
		const auto  posData = reinterpret_cast<const float*>(posBuffer.data.data() + posView.byteOffset + posAccessor.byteOffset);

		const auto& indexAccessor = model.accessors[primitive.indices];
		const auto& indexView     = model.bufferViews[indexAccessor.bufferView];
		const auto& indexBuffer   = model.buffers[indexView.buffer];
		const auto  indexData = reinterpret_cast<const uint16_t*>(indexBuffer.data.data() + indexView.byteOffset + indexAccessor.byteOffset);

		const int32_t nComponents = posAccessor.componentType == TINYGLTF_TYPE_SCALAR ? 1 : posAccessor.componentType == TINYGLTF_TYPE_VEC2 ? 2 : 3;

		mesh.vertices.reserve(posAccessor.count * nComponents);
		mesh.indices.reserve(indexAccessor.count);

		const size_t v_size = posAccessor.count * nComponents;
		const size_t i_size = indexAccessor.count;

		for (size_t i = 0; i < v_size; ++i)
			mesh.vertices.push_back(posData[i]);

		for (size_t i = 0; i < i_size; ++i)
			mesh.indices.push_back(indexData[i]);
	}

	bool LoadglTFModel(const std::string_view filepath, Object::Mesh& mesh)
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

		//SubmitModel(model);
		CheckModel(model, mesh);

		return success;
	}

}