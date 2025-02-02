#include "TestDirectoryWatcher.h"
#include <algorithm>

/*inline void FileSystemListener::_OnMoved(std::string dir, std::string filename, std::string old_filename)
{
	std::lock_guard<std::mutex> access(_access);
	const auto& movedEventQueue = _event_container[efsw::Action::Moved];
	if (_moved_callback) _moved_callback(dir, filename, old_filename);
}

inline void FileSystemListener::_OnModified(std::string dir, std::string filename, std::string old_filename)
{
	std::lock_guard<std::mutex> access(_access);
	const auto& modifiedEventQueue = _event_container[efsw::Action::Modified];
	if (_modifed_callback) _modifed_callback(dir, filename, old_filename);
}

inline void FileSystemListener::_OnDeleted(std::string dir, std::string filename, std::string old_filename)
{
	std::lock_guard<std::mutex> access(_access);
	const auto& deletedEventQueue= _event_container[efsw::Action::Delete];

	auto it = std::find_if(events.begin(), events.end(), [](const FileEvent& event) {
		return event.getType() == FileEvent::MOVED && event.getPath() == "/path/from/file" && event.getFileName() == "file1.txt";
		});

	if (it != events.end()) {
		events.erase(it);
		std::cout << "Event removed." << std::endl;
	}
	else {
		std::cout << "Event not found." << std::endl;

	}
	if (_deleted_callback) _deleted_callback(dir, filename, old_filename);
}

inline void FileSystemListener::_OnAdded(std::string dir, std::string filename, std::string old_filename)
{
	std::lock_guard<std::mutex> access(_access);
	const auto& addedEventQueue = _event_container[efsw::Action::Add];
	if (_added_callback) _added_callback(dir, filename, old_filename);
}*/

/*std::unique_lock<std::mutex>&& FileSystemListener::Lock()
{
	return std::unique_lock<std::mutex>(_access);
}

void FileSystemListener::AddEventModifiedInfo(EventInfo event_info)
{
	_event_container[efsw::Action::Modified].push_back(event_info);
}

void FileSystemListener::AddEventAddInfo(EventInfo event_info)
{
	_event_container[efsw::Action::Add].push_back(event_info);
}

void FileSystemListener::AddEventDeleteInfo(EventInfo event_info)
{
	_event_container[efsw::Action::Delete].push_back(event_info);
}

void FileSystemListener::AddEventMoveInfo(EventInfo event_info)
{
	_event_container[efsw::Action::Moved].push_back(event_info);
}*/

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
	//return;
}

lock_proxy<std::mutex, std::unordered_map<efsw::Action, std::deque<EventInfo>>>&& FileSystemListener::Lock()
{
	return ;
	//return lock_proxy<std::mutex, std::unordered_map<efsw::Action, std::deque<EventInfo>>> && ();
}

void FileSystemListener::AddPredicateCallback(_path_predicate prediacte, _callback_t callback)
{
	_filesystemEntryCallback.push_back({ prediacte,callback });
}
