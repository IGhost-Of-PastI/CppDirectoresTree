#pragma once
#include<filesystem>
#include<unordered_map>
#include<memory>
#include<optional>
#include<iostream>
#include<cassert>

//class DirectoriesTree;

template<class D, D defaultD, class F>
class DirectoryNode : public std::enable_shared_from_this<DirectoryNode<D,defaultD,F>>
{
public:
    friend typename F;
   // DirectoryNode() = default;
    DirectoryNode(std::filesystem::path folder, D data=defaultD, std::weak_ptr<DirectoryNode> parent=std::weak_ptr<DirectoryNode>());
    DirectoryNode& operator[](std::string folderName);
    //DirectoryNode& operator=(D data);
private:
    void _LoadChilds();
    std::filesystem::directory_entry _currFolder;
    std::weak_ptr<DirectoryNode> _parent;
    D _data;
    std::optional<std::unordered_map<std::string, std::shared_ptr<DirectoryNode>>> _childs;
};

template<class D, D defaultD, class F>
inline DirectoryNode<D, defaultD, F>::DirectoryNode(std::filesystem::path folder, D data, std::weak_ptr<DirectoryNode<D,defaultD,F>> parent)
{
    _currFolder.assign(folder);
    _parent = parent;
    _data = data;
}

template<class D, D defaultD, class F>
inline DirectoryNode<D, defaultD, F>& DirectoryNode<D, defaultD, F>::operator[](std::string folderName)
{
    if (!_childs.has_value())
    {
        std::unordered_map<std::string, std::shared_ptr<DirectoryNode>> map;
        _childs = map;
        _LoadChilds();
    }
    return *_childs.value()[folderName];
}

template<class D, D defaultD, class F>
inline void DirectoryNode<D, defaultD, F>::_LoadChilds()
{
    using namespace std;
    using namespace std::filesystem;

    std::unordered_map<std::string, std::shared_ptr<DirectoryNode>> childsMap;
    _childs = childsMap;
    for (const directory_entry& entry : directory_iterator(_currFolder.path()))
    {
        if (entry.is_directory())
        {
            //shared_from_this();
            //shared_from_this();
            //auto childNode= make_shared<DirectoryNode>(entry.path(), defaultD, shared_from_this());
            auto childNode = std::make_shared<DirectoryNode<D, defaultD, F>>(entry.path(), defaultD, this->weak_from_this());
            _childs.value().emplace(entry.path().filename().string(), childNode);
            //DirectoryNode childNode(entry.path(), defaultD, shared_from_this());
            //_childs.value()[entry.path().filename().string()] = make_shared<DirectoryNode>(childNode);
        }
    }
}
