#include "TestDirectoryWatcher.h"
#include <algorithm>
#include <iostream>


inline void FileSystemListener::_OnFileSystemEvent(efsw::Action action, const std::filesystem::path& path, std::string filename, std::string old_filename)
{
	std::lock_guard<std::mutex> guard(_access);
	auto& eventInfoQueue = _event_container[action];
	EventInfo sample = { path,filename,old_filename };
	auto element = std::find_if(eventInfoQueue.begin(), eventInfoQueue.end(), [&sample](const EventInfo& eventInf) {return sample == eventInf; });
	if (element == eventInfoQueue.end()) {
		for (const auto& [predicate, handler] : _filesystemEntryCallback) {
			if (predicate(std::filesystem::path(path)/filename)) { 
				handler(action, path, filename, old_filename); 
				break;
			}
		}
	}
	else
	{
		eventInfoQueue.erase(element);
	}
}

FileSystemWatcherEventProxy&& FileSystemListener::Lock()
{
	return FileSystemWatcherEventProxy(_event_container,_access);
}

void FileSystemListener::AddPredicateCallback(fspath_predicate_t prediacte, fscallback_t callback)
{
	_filesystemEntryCallback.push_back({ prediacte,callback });
}

void FileSystemListener::handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename)
{
	using namespace std;
	cout << endl;
	cout << filename<<endl;
	string utf8string = boost::locale::conv::to_utf<char>(filename,"UTF-8");
	cout << utf8string << endl;
	cout << endl;
	_OnFileSystemEvent(action, dir, filename, oldFilename);
}
