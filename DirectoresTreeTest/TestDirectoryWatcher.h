#pragma once
#include <efsw/efsw.hpp>
#include <thread>
#include <functional>
#include <iostream>

class TestDirectoryListener : public efsw::FileWatchListener
{
private:
    using _callback_t = std::function<void(std::string, std::string, std::string)>;
    _callback_t _added_callback;
    _callback_t _deleted_callback;
    _callback_t _modifed_callback;
    _callback_t _moved_callback;
public:
    TestDirectoryListener(_callback_t added_callback, _callback_t deleted_callback, _callback_t modifed_callback, _callback_t moved_callback)
    {
        _added_callback = added_callback;
        _deleted_callback = deleted_callback;
        _modifed_callback = modifed_callback;
        _moved_callback = moved_callback;
    }
    void handleFileAction(efsw::WatchID watchid, const std::string& dir,
        const std::string& filename, efsw::Action action,
        std::string oldFilename) override {
        using namespace efsw;
        using namespace std;
        cerr << this_thread::get_id() << endl;
        switch (action) {
        case Actions::Add:
            if (_added_callback) _added_callback(dir,filename,oldFilename);
            break;
        case Actions::Delete:
            if (_deleted_callback) _deleted_callback(dir, filename, oldFilename);
            break;
        case Actions::Modified:
            if (_modifed_callback) _modifed_callback(dir, filename, oldFilename);
            break;
        case Actions::Moved:
            if (_moved_callback) _moved_callback(dir, filename, oldFilename);
            break;
        }
    }
};

