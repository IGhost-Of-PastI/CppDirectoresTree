#include "FilesMap.h"

FilesMap::FilesMap(const std::filesystem::path& filesDir)
{
	using namespace std;
	using namespace std::filesystem;

	if (filesDir.is_absolute() && exists(filesDir))
	{
		directory_iterator dirIter(filesDir);
		for (const directory_entry& entry : dirIter)
		{
			if (entry.is_regular_file())
			{
				_filesMap.emplace(entry.path().filename().string(),entry);
			}
		}
	}
}
