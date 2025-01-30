#include <iostream>  
#include <filesystem>

using namespace std;
//#include "rproperty.h"
/*#include <memory>  
#include <list>  
#include <filesystem>  
#include <thread>  
#include <optional>  
#include <atomic>  
#include <chrono>  
#include <future>  

namespace fs = std::filesystem;

class DirectoryTreeNode
{
public: 
    DirectoryTreeNode(fs::path dirPath);
private:
    fs::path _directoryPath;
    std::optional<std::list<std::shared_ptr<DirectoryTreeNode>>> _childs;

    //Дополнительные параметры
    
};

class DirectoresTree
{
    public:
        DirectoresTree(fs::path rootDirPath);
        DirectoresTree();
        virtual ~DirectoresTree() = default;
        void setRootPath(fs::path rootDirPath);
    private:
        std::shared_ptr<DirectoryTreeNode> _root;
};*/







/*template <typename T>
class FolderTreeNode : public std::enable_shared_from_this<FileTreeNode<T>> {
public:
    FileTreeNode(const fs::path& folderPath, std::optional<T> extraData = std::nullopt)
        : path(folderPath), data(extraData) {
    }

    fs::path getPath() const {
        return path;
    }

    std::optional<T> getData() const {
        return data;
    }

    void lazyInitializeChildren() {
        if (children.empty()) {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_directory()) {
                    auto childNode = std::make_shared<FileTreeNode<T>>(entry.path());
                    children.push_back(childNode);
                }
            }
        }
    }

    const std::vector<std::shared_ptr<FileTreeNode>>& getChildren() const {
        return children;
    }

    void clearChildren() {
        children.clear();
    }

    std::shared_ptr<FileTreeNode> findNode(const fs::path& targetPath) {
        if (path == targetPath) {
            return this->shared_from_this(); // Используем shared_from_this для получения shared_ptr  
        }

        for (const auto& child : children) {
            auto result = child->findNode(targetPath);
            if (result) {
                return result;
            }
        }
        return nullptr; // Если не нашли, возвращаем nullptr  
    }

    int getDepth(const fs::path& targetPath, int depth = 0) {
        if (path == targetPath) {
            return depth; // Если узел найден, возвращаем текущую глубину  
        }

        for (const auto& child : children) {
            int childDepth = child->getDepth(targetPath, depth + 1); // Рекурсивно проверяем глубину дочерних узлов  
            if (childDepth != -1) {
                return childDepth; // Если нашли глубину, возвращаем её  
            }
        }
        return -1; // Если узел не найден, возвращаем -1  
    }

private:
    fs::path path;
    std::optional<T> data; // Дополнительные данные  
    std::vector<std::shared_ptr<FileTreeNode>> children; // Дочерние узлы  
};*/

/*template <typename T>
class FileSystemTree {
public:
    FileSystemTree(const fs::path& rootPath) : root(std::make_shared<FileTreeNode<T>>(rootPath)), isMonitoring(false) {}

    void printTree() {
        printFileTree(root);
    }

    void startMonitoring(std::chrono::milliseconds interval) {
        isMonitoring = true;
        monitoringFuture = std::async(std::launch::async, &FileSystemTree::monitorChanges, this, interval);
    }

    void stopMonitoring() {
        isMonitoring = false;
        if (monitoringFuture.valid()) {
            monitoringFuture.get(); // Ждем завершения потока  
        }
    }

    std::shared_ptr<FileTreeNode<T>> findNode(const fs::path& targetPath) {
        return root->findNode(targetPath);
    }

    int getNodeDepth(const fs::path& targetPath) {
        return root->getDepth(targetPath); // Получаем глубину от корневого узла  
    }

private:
    std::shared_ptr<FileTreeNode<T>> root;
    std::future<void> monitoringFuture;
    std::atomic<bool> isMonitoring;

    void printFileTree(const std::shared_ptr<FileTreeNode<T>>& node, int depth = 0) {
        if (!node) return;

        std::cout << std::string(depth * 2, ' ') << node->getPath().filename().string() << "\n";
        node->lazyInitializeChildren();

        for (const auto& child : node->getChildren()) {
            printFileTree(child, depth + 1);
        }
    }

    void monitorChanges(std::chrono::milliseconds interval) {
        while (isMonitoring) {
            checkForChanges(root);
            std::this_thread::sleep_for(interval);
        }
    }

    void checkForChanges(const std::shared_ptr<FileTreeNode<T>>& node) {
        if (fs::exists(node->getPath())) {
            node->lazyInitializeChildren();
        }
        else {
            std::cout << "Directory " << node->getPath() << " has been deleted.\n";
            node->clearChildren(); // Чистим детей в случае удаления узла  
        }
    }
};*/
#include "DirectoresTree.h"

int main() {
    using namespace std::filesystem;
    
    DirectoresTree dirTree=current_path().parent_path().append("x64").append("Debug");

    //path testfolder="TestFolder";

    //if (is_directory(absolute(testfolder)) || exists(absolute(testfolder)))
    //{
    //    cout << testfolder << endl;
    //    cout << absolute(testfolder) << endl;
    //}



    path currfolder = current_path().parent_path().append("x64");

    cout << currfolder << endl;

    recursive_directory_iterator rDirIter(currfolder);

    for (const auto& entry : rDirIter)
    {
        
        if (entry.is_directory())
        {
            path path1 = entry;
            cout << path1.string()<< "\t" <<rDirIter.depth()<< endl;
        }
    }

    //cout << testfolder << endl;
    //cout << absolute(testfolder) << endl;

    //test.field = 1;
   // rproperty t;

    //rproperty<int> t = 1;
    /*fs::path rootPath = "path/to/your/directory"; // Укажите путь к нужной директории  
    FileSystemTree<std::string> fileSystemTree(rootPath);

    // Запускаем асинхронный мониторинг изменений  
    fileSystemTree.startMonitoring(std::chrono::seconds(5));

    // Печатаем начальное дерево  
    fileSystemTree.printTree();

    std::cout << "Monitoring... Press Enter to stop.\n";
    std::cin.get(); // Ожидание ввода для остановки мониторинга  

    // Поиск узлов по пути  
    std::string searchPath;
    std::cout << "Enter path to search for a node: ";
    std::getline(std::cin, searchPath);
    auto foundNode = fileSystemTree.findNode(searchPath);
    if (foundNode) {
        std::cout << "Node found: " << foundNode->getPath() << std::endl;
    }
    else {
        std::cout << "Node not found." << std::endl;
    }

    // Получение глубины узла по пути  
    int depth = fileSystemTree.getNodeDepth(searchPath);
    if (depth != -1) {
        std::cout << "Depth of the node: " << depth << std::endl;
    }
    else {
        std::cout << "Node not found, so depth cannot be determined." << std::endl;
    }

    // Останавливаем мониторинг  
    fileSystemTree.stopMonitoring();
    */


    return 0;
}

/*DirectoresTree::DirectoresTree()
{
    auto path = fs::current_path();
}*/
