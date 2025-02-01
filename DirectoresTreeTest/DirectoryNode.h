#pragma once
#include<filesystem>
#include<unordered_map>
#include<memory>
#include<optional>
//#include<iostream>
//#include<cassert>

//class DirectoriesTree;

template<class D>
class DirectoryNode : public std::enable_shared_from_this<DirectoryNode<D>>
{
public:
    //friend typename F;
   // DirectoryNode() = default;
    DirectoryNode(const std::filesystem::path& folder, std::weak_ptr<DirectoryNode> parent=std::weak_ptr<DirectoryNode>());
    DirectoryNode operator[](std::wstring folderName);

    bool Rename(std::wstring newName);
    bool CreateChild(std::wstring dirName);
    bool DeleteChild(std::wstring childName);
    //DirectoryNode& operator=(D data);
private:
    void _LoadChilds();
    void _UpdateChildPathR();
    std::filesystem::directory_entry _currFolder;
    std::weak_ptr<DirectoryNode> _parent;
    D _data;
    std::optional<std::unordered_map<std::wstring, std::shared_ptr<DirectoryNode>>> _childs;
};

template<class D>
inline DirectoryNode<D>::DirectoryNode(const std::filesystem::path& folder, std::weak_ptr<DirectoryNode<D>> parent)
{
    _currFolder.assign(folder);
    _parent = parent;
}

template<class D>
inline DirectoryNode<D>& DirectoryNode<D>::operator[](std::wstring folderName)
{
    if (!_childs.has_value())
    {
        //std::unordered_map<std::string, std::shared_ptr<DirectoryNode>> map;
        //_childs = map;
        _LoadChilds();
    }
    return *_childs.value()[folderName];
}

template<class D>
inline void DirectoryNode<D>::_LoadChilds()
{
    using namespace std;
    using namespace std::filesystem;

    std::unordered_map<std::string, std::shared_ptr<DirectoryNode>> childsMap;
    _childs = childsMap;
    const path currFolder = _currFolder.path();
    for (const directory_entry entry : directory_iterator(currFolder))
    {
        if (entry.is_directory())
        {
            auto childNode = std::make_shared<DirectoryNode<D>>(entry.path(), this->weak_from_this());
            _childs.value().emplace(entry.path().filename().string(), childNode);
        }
    }
}
