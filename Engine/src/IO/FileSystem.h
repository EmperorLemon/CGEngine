#pragma once

#include <string_view>
#include <vector>

namespace CGEngine::IO
{
	enum class FileType : uint8_t
	{
		TEXT_FILE,
		BINARY_FILE
	};

	enum class ModelFileType : uint8_t
	{
		glTF,
		glb,
		OBJ,
	};

	void LoadModelFile(std::string_view filepath, ModelFileType type);

	void ReadFile(std::string_view filepath, std::vector<std::byte>& data);
	void ReadFile(std::string_view filepath, std::string& data);

	bool FileExists(std::string_view filepath);
}
