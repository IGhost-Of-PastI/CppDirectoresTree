#pragma once

#include "DirectoryNode.h"
#include <filesystem>
#include <memory>
#include <stack>

template<class D, D defaultD>
class DirectoriesTree
{
   
public:
    using NodeType = DirectoryNode<D, defaultD, DirectoriesTree>;
    DirectoriesTree(std::filesystem::path rootDir = std::filesystem::current_path());
    //depth
   // void SetRootDir(std::filesystem::path newRootDir);
      std::filesystem::path GetRootPath()const;
      NodeType& operator[] (std::filesystem::path dir);
private:
    std::shared_ptr<NodeType> _rootNode;
    //std::shared_ptr<NodeType> _selected;
};

template<class D, D defaultD>
inline DirectoriesTree<D, defaultD>::DirectoriesTree(std::filesystem::path rootDir)
{
    _rootNode = std::make_shared<NodeType>(rootDir);
    //_selected = _rootNode;
}

template<class D, D defaultD>
inline std::filesystem::path DirectoriesTree<D, defaultD>::GetRootPath() const
{
    return _rootNode->_currFolder;
}



template<class D, D defaultD>
inline typename DirectoriesTree<D,defaultD>::NodeType& DirectoriesTree<D, defaultD>::operator[](std::filesystem::path dir)
{
    std::filesystem::path rootFolder = _rootNode->_currFolder;
    if (dir.root_path() == rootFolder.root_path())
    {
        std::stack<std::string> folders;
        
        bool isFound = false;
        while ( !isFound)
        {
            folders.push(dir.filename().string());
            dir = dir.parent_path();
            if (dir.filename() == rootFolder.filename())
            {
                isFound = true;
            }
        }
        NodeType& node = *_rootNode;
        while (!folders.empty())
        {
            node = node[folders.top()];
            folders.pop();
        }
        return node;
    }
}

//template<class D, D defaultD>
//inline DirectoriesTree<D, defaultD>::NodeType& DirectoriesTree<D, defaultD>::operator[](std::filesystem::path dir)
//{
    //return nullptr;
//}
