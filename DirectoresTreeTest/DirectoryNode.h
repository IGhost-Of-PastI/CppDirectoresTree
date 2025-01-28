#pragma once
#include<filesystem>
#include<unordered_map>
#include<memory>
#include<optional>

class DirectoryNode
{
public:
	DirectoryNode(std::filesystem::path folder);
private:
	std::filesystem::path _currFolder;
	std::optional<std::unordered_map<std::filesystem::path, std::shared_ptr<DirectoryNode>>> _childs;
};

