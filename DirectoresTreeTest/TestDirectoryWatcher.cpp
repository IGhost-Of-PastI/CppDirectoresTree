#include "FileSystemListener.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/locale.hpp>
//#include <codecvt>

inline std::string To_UTF8(std::string w1251_str)
{
	using namespace boost::locale::conv;
	return between(w1251_str, "WINDOWS-1251", "UTF-8");
}

inline void FileSystemListener::_OnFileSystemEvent(efsw::Action action, const std::filesystem::path& path, std::string filename, std::string old_filename)
{
	std::lock_guard<std::mutex> guard(_access);
	auto& eventInfoQueue = _event_container[action];
	EventInfo sample = { path,filename,old_filename };
	auto element = std::find_if(eventInfoQueue.begin(), eventInfoQueue.end(), [&sample](const EventInfo& eventInf) {return sample == eventInf; });
	if (element == eventInfoQueue.end()) {
		for (const auto& [predicate, handler] : _filesystemEntryCallback) {
			//Значительный недостаток, тогда она не сомждет определить верный предикат, тоесть нужно заготовить варниты предикатов, скорее всгео эжто убдут встреоныне функции
			//bool checkPredicate = (action == efsw::Action::Delete) ? !predicate(std::filesystem::path(path) / filename) : predicate(std::filesystem::path(path) / filename);
			if (predicate(action,std::filesystem::path(path) / filename)) {
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
	//using namespace boost::locale::conv;
	//cout << endl;
	//cout << filename<<endl;
	//cout<<boost::locale::conv::between(filename, "WINDOWS-1251", "UTF-8")<<endl;
	//cout << endl;
	_OnFileSystemEvent(action, To_UTF8(dir), To_UTF8(filename), To_UTF8(oldFilename));
}
