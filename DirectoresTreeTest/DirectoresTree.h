#pragma once

#include "DirectoryNode.h"
#include <filesystem>
#include <memory>

class DirectoriesTree
{
public:
    DirectoriesTree(std::filesystem::path rootDir = std::filesystem::current_path());
    //depth
    void setRootDir(std::filesystem::path newRootDir);
    DirectoryNode& operator[] (std::filesystem::path dir);
private:
    std::shared_ptr<DirectoryNode> _rootNode;
    std::shared_ptr<DirectoryNode> _selected;
};

