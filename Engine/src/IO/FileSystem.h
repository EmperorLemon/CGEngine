#pragma once

#include <string>
#include <vector>

namespace CGEngine::IO
{
	void ReadFile(const char* filepath, std::vector<std::byte>& data);
	void ReadFile(const char* filepath, std::string& data);

	bool FileExists(const char* filepath);
}
