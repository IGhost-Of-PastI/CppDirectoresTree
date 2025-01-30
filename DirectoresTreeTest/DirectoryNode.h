#pragma once
#include<filesystem>
#include<unordered_map>
#include<memory>
#include<optional>

class DirectoriesTree;

class DirectoryNode : public std::enable_shared_from_this<DirectoryNode>
{
public:
    friend DirectoriesTree;
    DirectoryNode(std::filesystem::path folder, std::weak_ptr<DirectoryNode> parent);
    DirectoryNode& operator[](std::string folderName);
private:
    void _LoadChilds();
    std::filesystem::directory_entry _currFolder;
    std::weak_ptr<DirectoryNode> _parent;
    std::optional<std::unordered_map<std::string, std::shared_ptr<DirectoryNode>>> _childs;
};

