#include "glTFLoader.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include "Core/Logger.hpp"

namespace CGEngine::IO
{
	static void SetupMesh(tinygltf::Model& model, tinygltf::Mesh& mesh)
	{
		std::shared_ptr<OpenGL::GLBuffer> vertex_buffer = nullptr;

		for (const auto& bufferView : model.bufferViews)
		{
			if (bufferView.target == 0)
			{
				CG_WARN("glTF Warning: bufferView.target is zero.");
				continue;
			}

			const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

			vertex_buffer = std::make_shared<OpenGL::GLBuffer>(bufferView.byteLength, buffer.data.data() + bufferView.byteOffset);
		}

		std::shared_ptr<OpenGL::GLVertexArray> vertex_array = nullptr;

		for (const auto& primitive : mesh.primitives)
		{
			tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

			VertexLayout layout;

			for (auto& attrib : primitive.attributes)
			{
				tinygltf::Accessor accessor = model.accessors[attrib.second];
				int32_t byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);

				CG_TRACE("Attribute: {0} | {1} | {2} | {3}", attrib.first.c_str(), attrib.second, accessor.type, byteStride);
				//layout.add()
			}
		}
	}

	static void SetupModelNodes(tinygltf::Model& model, const tinygltf::Node& node)
	{
		if (node.mesh >= 0 && node.mesh < static_cast<int>(model.meshes.size()))
			SetupMesh(model, model.meshes[node.mesh]);

		for (const int i : node.children)
			SetupModelNodes(model, model.nodes[i]);
	}

	static void SetupModel(tinygltf::Model& model)
	{
		const tinygltf::Scene& scene = model.scenes[model.defaultScene];

		for (const int node : scene.nodes)
			SetupModelNodes(model, model.nodes[node]);
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

		SetupModel(model);

		return success;
	}

}