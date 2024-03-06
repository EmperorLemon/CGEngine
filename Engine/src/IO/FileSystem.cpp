#include "FileSystem.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "Core/Logger.hpp"
#include "Component/Mesh.h"

#include "IO/glTF/glTFLoader.h"

namespace CGEngine::IO
{
	void LoadModelFile(const std::string_view filepath, const ModelFileType type, Object::Mesh& mesh)
	{
		if (type == ModelFileType::glTF)
			LoadglTFModel(filepath, mesh);
	}

	void ReadFile(const std::string_view filepath, std::vector<std::byte>& data)
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