#include "AssimpLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Core/Logger.hpp"
#include "IO/FileSystem.h"

#include "Renderer/Assets/Model.h"

namespace CGEngine::IO
{
	constexpr uint32_t ASSIMP_LOADER_OPTIONS = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_ImproveCacheLocality | 
											   aiProcess_FixInfacingNormals | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes |
											   aiProcess_OptimizeGraph | aiProcess_SplitLargeMeshes;

	static void ExtractModel(const aiScene& scene, Assets::Model& model);

	static void ExtractMeshes(const aiScene& scene, Assets::Model& model);
	static void ProcessMesh(const aiMesh* assimpMesh, Assets::Mesh& mesh);

	static void ExtractMaterials(const aiScene& scene, Assets::Model& model);
	static void ProcessMaterial(const aiMaterial* assimpMaterial, Assets::Material& material);

	static void ExtractTextures(const aiMaterial* material, const aiTextureType type, Assets::Model& model);

	static std::string ASSET_DIRECTORY_PATH;

	void AssimpLoadModel(const std::string_view filepath, Assets::Model& model)
	{
		auto importer = Assimp::Importer{};

		const auto scene = importer.ReadFile(filepath.data(), ASSIMP_LOADER_OPTIONS);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			CG_ERROR("Assimp Error: {0}", importer.GetErrorString());

		ASSET_DIRECTORY_PATH = GetDirectoryPath(filepath) + "/";

		try
		{
			ExtractModel(*scene, model);
		}
		catch (std::exception& e)
		{
			CG_ERROR("{0}", e.what());
		}
	}

	void ExtractModel(const aiScene& scene, Assets::Model& model)
	{
		if (scene.HasMeshes()) ExtractMeshes(scene, model);
		if (scene.HasMaterials()) ExtractMaterials(scene, model);
	}

	void ExtractMeshes(const aiScene& scene, Assets::Model& model)
	{
		Assets::Mesh mesh = {};

		for (unsigned int i = 0; i < scene.mNumMeshes; ++i)
		{
			const auto& assimpMesh = scene.mMeshes[i];

			ProcessMesh(assimpMesh, mesh);

			model.meshes.emplace_back(std::move(mesh));
		}
	}

	void ProcessMesh(const aiMesh* assimpMesh, Assets::Mesh& mesh)
	{
		const bool HAS_NORMALS   = assimpMesh->HasNormals();
		const bool HAS_TEXCOORDS = assimpMesh->HasTextureCoords(0);

		if (assimpMesh->HasFaces())
		{
			for (unsigned int i = 0; i < assimpMesh->mNumFaces; ++i)
			{
				const auto indexData = assimpMesh->mFaces[i].mIndices;
				const auto indexCount = assimpMesh->mFaces[i].mNumIndices;

				mesh.indices.insert(mesh.indices.end(), indexData, indexData + indexCount);
			}
		}

		mesh.vertices.reserve(assimpMesh->mNumVertices);

		mesh.layout.add(0, 3, DataType::FLOAT, 0);
		mesh.layout.add(1, 3, DataType::FLOAT, 3 * sizeof(float));
		mesh.layout.add(2, 2, DataType::FLOAT, 6 * sizeof(float));
		mesh.layout.SetStride(8 * sizeof(float));

		for (unsigned int i = 0; i < assimpMesh->mNumVertices; ++i)
		{
			mesh.vertices.emplace_back(assimpMesh->mVertices[i].x);
			mesh.vertices.emplace_back(assimpMesh->mVertices[i].y);
			mesh.vertices.emplace_back(assimpMesh->mVertices[i].z);

			if (HAS_NORMALS)
			{
				mesh.vertices.emplace_back(assimpMesh->mNormals[i].x);
				mesh.vertices.emplace_back(assimpMesh->mNormals[i].y);
				mesh.vertices.emplace_back(assimpMesh->mNormals[i].z);
			}

			if (HAS_TEXCOORDS)
			{
				mesh.vertices.emplace_back(assimpMesh->mTextureCoords[0][i].x);
				mesh.vertices.emplace_back(assimpMesh->mTextureCoords[0][i].y);
			}
		}

	}

	void ExtractMaterials(const aiScene& scene, Assets::Model& model)
	{
		Assets::Material material = {};

		for (unsigned int i = 0; i < scene.mNumMaterials; ++i)
		{
			const auto& assimpMaterial = scene.mMaterials[i];

			ProcessMaterial(assimpMaterial, material);
			ExtractTextures(assimpMaterial, aiTextureType_DIFFUSE, model);

			model.materials.emplace_back(material);
		}
	}

	void ProcessMaterial(const aiMaterial* assimpMaterial, Assets::Material& material)
	{
		aiColor4D albedo;

		assimpMaterial->Get(AI_MATKEY_BASE_COLOR, albedo);

		material.albedo = Math::Vec4(albedo.r, albedo.g, albedo.b, albedo.a);
	}

	void ExtractTextures(const aiMaterial* material, const aiTextureType type, Assets::Model& model)
	{
		const auto count = material->GetTextureCount(type);

		for (unsigned int i = 0; i < count; ++i)
		{
			auto importedPath = aiString{};

			if (material->GetTexture(type, i, &importedPath) == AI_SUCCESS)
			{
				const std::string path = ASSET_DIRECTORY_PATH + importedPath.C_Str();

				Image image = {};
				LoadImageFile(path.c_str(), image.width, image.height, image.channels, image.pixels, false);
				model.textures.emplace_back(std::move(image));
			}
		}
	}
}
