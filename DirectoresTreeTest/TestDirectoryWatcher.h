#pragma once
#include <efsw/efsw.hpp>
//#include <thread>
#include <functional>
//#include <iostream>
//#include <boost/multi_index_container.hpp>
//#include <shared_mutex>
#include <mutex>
#include <string>
#include <deque>
#include <unordered_map>
#include "lock_proxy.h"
#include <filesystem>
#include <utility>
#include <list>
//#include <>



struct EventInfo
{
    std::filesystem::path dir;
    std::string filename;
    std::string old_filename;
    //auto operator<=>(const EventInfo&) const = default;
    bool operator==(const EventInfo& other) const {
        return (dir == other.dir) && (filename == other.filename) && (old_filename == other.old_filename);
    }
    /*auto operator<=>(const EventInfo& other) const {
        if (auto cmp = dir <=> other.dir; cmp != 0) return cmp;
        if (auto cmp = filename <=> other.filename; cmp != 0) return cmp;
        return old_filename <=> other.old_filename;
    }*/
};

using eventInfoContainer = std::unordered_map<efsw::Action, std::deque<EventInfo>>;

class FileSystemWatcherEventProxy
{
public:
    FileSystemWatcherEventProxy(eventInfoContainer& container, std::unique_lock<std::mutex> guard)
        : _container(container), _guard(std::move(guard)) {
    }
    FileSystemWatcherEventProxy(FileSystemWatcherEventProxy&& other) noexcept
        : _container(other._container), _guard(std::move(other._guard)){}

    FileSystemWatcherEventProxy& operator=(FileSystemWatcherEventProxy&& other) noexcept
    {
        if (this != &other)
        {
            _container = other._container;
            _guard = std::move(other._guard);
        }
        return *this;
    }
    void AddEventInfo(efsw::Action action, EventInfo eventInfo) {
        _container.emplace(action, eventInfo);
    };
private:
    eventInfoContainer& _container;
    std::unique_lock<std::mutex> _guard;
};

class FileSystemListener : public efsw::FileWatchListener
{
private:
    using _callback_t = std::function<void(efsw::Action,const std::filesystem::path&, std::string, std::string)>;
    using _path_predicate = std::function<bool(const std::filesystem::path&)>;
    
    std::list<std::pair<_path_predicate, _callback_t>> _filesystemEntryCallback;
    
    //_callback_t _added_callback;
    //_callback_t _deleted_callback;
    //_callback_t _modifed_callback;
    //_callback_t _moved_callback;
    
    eventInfoContainer _event_container;
    std::mutex _access;

    
    inline void _OnFileSystemEvent(efsw::Action, const std::filesystem::path&, std::string, std::string);
    //inline void _OnMoved(std::string dir,std::string filename,std::string old_filename);
    //inline void _OnModified(std::string dir, std::string filename, std::string old_filename);
    //inline void _OnDeleted(std::string dir, std::string filename, std::string old_filename);
    //inline void _OnAdded(std::string dir, std::string filename, std::string old_filename);
public:
    FileSystemWatcherEventProxy&& Lock();
    void AddPredicateCallback(_path_predicate,_callback_t);
    //void AddEventModifiedInfo(EventInfo event_info);
    //void AddEventAddInfo(EventInfo event_info);
    //void AddEventDeleteInfo(EventInfo event_info);
    //void AddEventMoveInfo(EventInfo event_info);

    //FileSystemListener(_callback_t added_callback, _callback_t deleted_callback, _callback_t modifed_callback, _callback_t moved_callback)
    //{
    //    _added_callback = added_callback;
    //    _deleted_callback = deleted_callback;
    //    _modifed_callback = modifed_callback;
    //    _moved_callback = moved_callback;
    //}
    void handleFileAction(efsw::WatchID watchid, const std::string& dir,
        const std::string& filename, efsw::Action action,
        std::string oldFilename) override {
        _OnFileSystemEvent(action, dir, filename, oldFilename);
        /*using namespace efsw;
        using namespace std;
        cerr << this_thread::get_id() << endl;
        switch (action) {
        case Actions::Add:
            _OnAdded(dir, filename, oldFilename);
            break;
        case Actions::Delete:
            _OnDeleted(dir, filename, oldFilename);
            break;
        case Actions::Modified:
            _OnModified(dir, filename, oldFilename);
            break;
        case Actions::Moved:
            _OnMoved(dir, filename, oldFilename);
            break;
        }*/
    }
};

