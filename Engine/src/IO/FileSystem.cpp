#include "FileSystem.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "Core/Logger.hpp"
#include "Renderer/Assets/Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "assimp/AssimpLoader.h"
#include "glTF/glTFLoader.h"

namespace CGEngine::IO
{
	void LoadImageFile(const std::string_view filepath, int& width, int& height, int& channels, std::vector<unsigned char>& data)
	{
		if (!FileExists(filepath))
			CG_ERROR("File {0} does not exist!", filepath.data());

		const auto pixels = stbi_load(filepath.data(), &width, &height, &channels, STBI_rgb_alpha);

		if (!pixels)
		{
			CG_ERROR("Unable to load image!");
			stbi_image_free(pixels);

			return;
		}

		const size_t size = static_cast<size_t>(width * height) * STBI_rgb_alpha;
		data.insert(data.end(), pixels, pixels + size);

		stbi_image_free(pixels);
	}

	void LoadModelFile(const std::string_view filepath, Assets::Model& model)
	{
		if (!FileExists(filepath))
			CG_ERROR("File {0} does not exist!", filepath.data());

		//LoadglTFModel(filepath, model);
		AssimpLoadModel(filepath, model);
	}

	void ReadFile(const std::string_view filepath, std::vector<unsigned char>& data)
	{
		if (std::ifstream file(filepath.data(), std::ios::binary | std::ios::ate); file)
		{
			file.seekg(0, std::ios::end);

			if (const auto size = file.tellg(); size != -1)
			{
				data.resize(size);
				file.seekg(0, std::ios::beg);
				file.read(reinterpret_cast<char*>(data.data()), size);
			}
			else
				CG_ERROR("Could not read file: {0}", filepath);
		}
		else
			CG_ERROR("Could not open file: {0}", filepath);
	}

	void ReadFile(const std::string_view filepath, std::string& data)
	{
		if (std::ifstream file(filepath.data(), std::ios::binary | std::ios::ate); file)
		{
			file.seekg(0, std::ios::end);

			if (const auto size = file.tellg(); size != -1)
			{
				data.resize(size);
				file.seekg(0, std::ios::beg);
				file.read(data.data(), size);
			}
			else
				CG_ERROR("Could not read file: {0}", filepath);
		}
		else
			CG_ERROR("Could not open file: {0}", filepath);
	}

	bool FileExists(const std::string_view filepath)
	{
		const auto s = std::filesystem::file_status{};
		return status_known(s) ? exists(s) : std::filesystem::exists(filepath);
	}
}
