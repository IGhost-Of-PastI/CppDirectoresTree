#include "DirectoryNode.h"

using namespace std;
using namespace std::filesystem;

DirectoryNode::DirectoryNode(std::filesystem::path folder, std::weak_ptr<DirectoryNode> parent)
{
    _currFolder.assign(folder);
    _parent = parent;
}

DirectoryNode& DirectoryNode::operator[](std::string folderName)
{
    if (!_childs.has_value())
    {
        std::unordered_map<std::string, std::shared_ptr<DirectoryNode>> map;
        _childs = map;
        _LoadChilds();
    }
    return *_childs.value()[folderName];
}

void DirectoryNode::_LoadChilds()
{
    std::unordered_map<std::string, std::shared_ptr<DirectoryNode>> childsMap;
    _childs = childsMap;
    for (const directory_entry& entry : directory_iterator(_currFolder.path()))
    {
        if (entry.is_directory())
        {
            DirectoryNode childNode(entry.path(), shared_from_this());
            _childs.value()[entry.path().filename().string()] = make_shared<DirectoryNode>(childNode);
            //_childs.value().insert()
        }
    }
}
