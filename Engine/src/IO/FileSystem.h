#pragma once

#include <vector>

namespace CGEngine::IO
{
	void ReadFile(const char* filepath, std::vector<char>& data);

	bool FileExists(const char* filepath);
}
