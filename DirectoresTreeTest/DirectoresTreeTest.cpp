#include <iostream>  
#include <filesystem>
#include "DirectoresTree.h"
#include <memory>

using namespace std;

int main() {
    using namespace std::filesystem;
    
    DirectoriesTree<int,10> dirTree=current_path().parent_path().append("x64").append("Debug");

    auto elm = dirTree[path().append("TestFolder").append("InnerFolder1")];/*current_path().parent_path().append("x64").append("Debug").append("TestFolder").append("InnerFolder1") * / ];

    cout << dirTree.GetRootPath() << endl;
    //dirTree[]
    //path currfolder = current_path().parent_path().append("x64");

    //cout << currfolder << endl;

//    recursive_directory_iterator rDirIter(currfolder);

 /*   for (const auto& entry : rDirIter)
    {
        
        if (entry.is_directory())
        {
            path path1 = entry;
            cout << path1.string()<< "\t" <<rDirIter.depth()<< endl;
        }
    }*/

    return 0;
}