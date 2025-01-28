#pragma once
#include<memory>
#include "DirectoryNode.h"

class DirectoresTree
{
public:
	DirectoresTree(std::filesystem::path rootDir);

	//depth
	void setRootDir(std::filesystem::path newRootDir);
	DirectoryNode& operator[] (std::filesystem::path dir);
private:
	std::shared_ptr<DirectoryNode> _rootNode;
};

