#include "glTFLoader.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include <stack>

#include "Core/Logger.hpp"

namespace CGEngine::IO
{
	constexpr auto PADDING = 2; // 2 bytes of padding needed to align memory

	std::shared_ptr<Buffer> vertex_buffer = nullptr;
	std::shared_ptr<VertexArray> vertex_array = nullptr;

	constexpr DataType GetDataType(const int32_t type)
	{
		switch (type)
		{
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			return DataType::UNSIGNED_BYTE;
		case TINYGLTF_COMPONENT_TYPE_BYTE:
			return DataType::BYTE;
		case TINYGLTF_COMPONENT_TYPE_SHORT:
			return DataType::SHORT;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			return DataType::UNSIGNED_SHORT;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			return DataType::UNSIGNED_INT;
		case TINYGLTF_COMPONENT_TYPE_INT:
			return DataType::INT;
		case TINYGLTF_COMPONENT_TYPE_FLOAT:
			return DataType::FLOAT;
		default:
			break;
		}

		return DataType::FLOAT;
	}

	static void ProcessMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
	{
		std::vector<SubBuffer> buffers;

		for (const auto& bufferView : model.bufferViews)
		{
			if (bufferView.target == 0)
			{
				CG_WARN("glTF Warning: bufferView target is 0");
				continue;
			}

			const auto& buffer = model.buffers[bufferView.buffer];
			buffers.emplace_back(bufferView.byteLength, bufferView.byteStride, bufferView.byteOffset, buffer.data.data() + bufferView.byteOffset);
		}

		size_t length = 0;

		for (const auto& buffer : buffers)
			length += buffer.length;

		vertex_buffer = std::make_shared<OpenGL::GLBuffer>(length + PADDING, nullptr);

		for (const auto& buffer : buffers)
			vertex_buffer->SetSubData(buffer.offset, buffer.length, buffer.data);

		VertexLayout layout;

		for (const auto& primitive : mesh.primitives)
		{
			tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

			for (const auto& attribute : primitive.attributes)
			{
				const tinygltf::Accessor accessor = model.accessors[attribute.second];

				layout.add(attribute.second, static_cast<int32_t>(accessor.count), GetDataType(accessor.componentType));
			}
		}

		layout.end();

		vertex_array = std::make_shared<OpenGL::GLVertexArray>(dynamic_cast<OpenGL::GLBuffer*>(vertex_buffer.get()), layout);

		//CG_TRACE("{0}", vertex_array->GetID());
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

	bool LoadglTFModel(const std::string_view filepath)
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

		SubmitModel(model);

		return success;
	}

}