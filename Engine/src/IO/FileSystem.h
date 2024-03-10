#pragma once

#include <string_view>
#include <vector>

namespace CGEngine::Assets
{
	struct Model;
}

namespace CGEngine::IO
{
	enum class ModelFileType : uint8_t
	{
		glTF,
		glb,
		OBJ,
	};

	void LoadImageFile(std::string_view filepath, int& width, int& height, int& channels, std::vector<unsigned char>& data);
	void LoadModelFile(std::string_view filepath, ModelFileType type, Assets::Model& model);

	void ReadFile(std::string_view filepath, std::vector<unsigned char>& data);
	void ReadFile(std::string_view filepath, std::string& data);

	bool FileExists(std::string_view filepath);
}
