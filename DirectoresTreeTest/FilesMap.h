#pragma once
#include <unordered_map>
#include <filesystem>

class FilesMap
{
public:
	FilesMap(const std::filesystem::path& filesDir);
private:
	std::unordered_map<std::string, std::filesystem::directory_entry> _filesMap;
};

