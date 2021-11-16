// Copyright (c) 2021 CoinGreen Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <shared_mutex>

#include <boost/filesystem/operations.hpp>

#include "policy/mining_policy.h"
#include "rpc/protocol.h"
#include "util.h"

std::set<std::string> defaultAllowedAddresses = {
     "D8V6a71DXgTiNEkYfpDnZH9wFfomHeKjym", // markus          
     "DTjNXq6evTztRAdRD6mqfGftLX359fixkQ" // maxirmx
 };


std::set<std::string>& allowedAddresses = defaultAllowedAddresses;

enum class FileStatus {created, modified, erased};

class FileWatcher {
 public:
     std::string toWatch;
     std::time_t modTime;
     std::chrono::duration<int, std::milli> delay;
 
     FileWatcher(std::string _toWatch, std::chrono::duration<int, std::milli> delay) : toWatch{_toWatch}, delay{delay}, modTime{0} {
     }

/*     void start(const std::function<void (std::string, FileStatus)> &action) {
         while(running_) {
             // Wait for "delay" milliseconds
             std::this_thread::sleep_for(delay);
 
             auto it = paths_.begin();
             while (it != paths_.end()) {
                 if (!std::filesystem::exists(it->first)) {
                     action(it->first, FileStatus::erased);
                     it = paths_.erase(it);
                 }
                 else {
                     it++;
                 }
             }
 
            // Check if a file was created or modified
             for(auto &file : std::filesystem::recursive_directory_iterator(path_to_watch)) {
                 auto current_file_last_write_time = std::filesystem::last_write_time(file);
 
                 // File creation
                 if(!contains(file.path().string())) {
                     paths_[file.path().string()] = current_file_last_write_time;
                     action(file.path().string(), FileStatus::created);
                 // File modification
                 } else {
                     if(paths_[file.path().string()] != current_file_last_write_time) {
                         paths_[file.path().string()] = current_file_last_write_time;
                         action(file.path().string(), FileStatus::modified);
                     }
                 }
             } 
         }
     }*/
 private:
//     std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
     bool running_ = true;
 
};




int IsMiningAllowed(const std::string& addr, std::string& reason) {
    int ret = RPC_NO_ERROR;
    auto search = allowedAddresses.find(addr);
    if (search == allowedAddresses.end()) {
        reason = std::string("Mining to address ") + addr + std::string(" is not allowed");
        ret = RPC_MINING_NOT_ALLOWED;
    }  
  return ret;
}

int IsMiningAllowed(const CBitcoinAddress& addr, std::string& reason) {
    return IsMiningAllowed(addr.ToString(), reason);
}

int IsMiningAllowed(const CScript& scrpt, std::string& reason) {
    bool ret = RPC_NO_ERROR;
    CTxDestination addrTo;
    if (!ExtractDestination(scrpt, addrTo)) {
        reason = std::string("Mining was not allowed since ExtractDestination failed for coinbase script. This is an internal error, please report it to development team");
        ret = RPC_INTERNAL_ERROR;
    }
    else {
        ret = IsMiningAllowed(CBitcoinAddress(addrTo), reason);
    }
    return ret;
}


