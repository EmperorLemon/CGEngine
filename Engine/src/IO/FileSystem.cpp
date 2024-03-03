#include "FileSystem.h"

#include "Core/Logger.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace CGEngine::IO
{
	void ReadFile(const char* filepath, std::vector<char>& data)
	{
		if (std::ifstream file(filepath, std::ios::ate | std::ios::binary); file)
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

	bool FileExists(const char* filepath)
	{
		const auto s = std::filesystem::file_status{};
		return status_known(s) ? exists(s) : std::filesystem::exists(filepath);
	}
}