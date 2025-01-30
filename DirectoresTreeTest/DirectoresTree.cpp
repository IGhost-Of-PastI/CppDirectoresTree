#include "DirectoresTree.h"

using namespace std;
using namespace std::filesystem;

DirectoriesTree::DirectoriesTree(std::filesystem::path rootDir)
{
	_rootNode = make_shared<DirectoryNode>(rootDir);
	_selected = _rootNode;
}

void DirectoriesTree::setRootDir(std::filesystem::path newRootDir)
{
}