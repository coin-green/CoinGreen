// Copyright (c) 2021 CoinGreen Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <shared_mutex>

#include "policy/filewatch.hpp"
#include "policy/mining_policy.h"
#include "rpc/protocol.h"
#include "util.h"

std::set<std::string> defaultAllowedAddresses = {
     "D8V6a71DXgTiNEkYfpDnZH9wFfomHeKjym", // markus          
     "DTjNXq6evTztRAdRD6mqfGftLX359fixkQ" // maxirmx
 };

std::set<std::string>& allowedAddresses = defaultAllowedAddresses;

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
    int ret = RPC_NO_ERROR;
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


