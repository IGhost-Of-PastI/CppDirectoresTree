#pragma once

#include "DirectoryNode.h"
#include "DirectoryChangesListener.h"
#include <filesystem>
#include <memory>
#include <stack>
#include <efsw/efsw.hpp>

template<class D>
class DirectoriesTree
{
   
public:
    using NodeType = DirectoryNode<D>;
    DirectoriesTree(std::filesystem::path rootDir = std::filesystem::current_path());
    //depth
    bool RenameDir(const std::filesystem::path& folderPath, std::wstring newName);
    bool CreateDir(const std::filesystem::path& parentPath, std::wstring folderName);
    bool DeleteDir(const std::filesystem::path& dirPath);
   // void SetRootDir(std::filesystem::path newRootDir);
      std::filesystem::path GetRootPath()const;
      NodeType& operator[] (std::filesystem::path dir);
private:
    std::shared_ptr<NodeType> _rootNode;
    std::unique_ptr<DirectoryChangesListener> _changesListener;
    std::unique_ptr<efsw::FileWatcher> _directoryWatcher;
    efsw::WatchID _watchID;
    //std::shared_ptr<NodeType> _selected;
};

template<class D>
inline DirectoriesTree<D>::DirectoriesTree(const std::filesystem::path rootDir)
{
    _rootNode = std::make_shared<NodeType>(rootDir);
    _directoryWatcher = std::make_unique<efsw::FileWatcher>();
    _changesListener = std::make_unique<DirectoryChangesListener>();
    efsw::WatcherOption option(efsw::Options::Option::WinBufferSize, 64 * 1024);
    //options.mOption = efsw::Options::Option::WinBufferSize;
    //std::vector<efsw::WatcherOption> optionslist = { option };
    //options.mValue = 64 * 1024;
    _watchID = _directoryWatcher->addWatch(rootDir.string(), _changesListener.get(), true, { option });
    _directoryWatcher->watch();
}

template<class D>
inline bool DirectoriesTree<D>::RenameDir(const std::filesystem::path& folderPath, std::wstring newName)
{
    return false;
}

template<class D>
inline bool DirectoriesTree<D>::CreateDir(const std::filesystem::path& parentPath, std::wstring folderName)
{
    return false;
}

template<class D>
inline bool DirectoriesTree<D>::DeleteDir(const std::filesystem::path& dirPath)
{
    return false;
}

template<class D>
inline std::filesystem::path DirectoriesTree<D>::GetRootPath() const
{
    return _rootNode->_currFolder;
}



template<class D>
inline typename DirectoriesTree<D>::NodeType& DirectoriesTree<D>::operator[](std::filesystem::path dir)
{
    if (!dir.is_absolute())
    {
        dir = std::filesystem::absolute(dir);
    }
    const std::filesystem::path rootFolder = _rootNode->_currFolder;
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
        //вот здесь ошибка поскольку это не константная ссылка на сам обект то присваивание данных фактчиески изменяет сам обект гораздо правильнее здесь получить константный указатель
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
