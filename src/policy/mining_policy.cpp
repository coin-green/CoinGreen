// Copyright (c) 2021 CoinGreen Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

#include "policy/file_watch.hpp"
#include "policy/mining_policy.h"
#include "rpc/protocol.h"
#include "util.h"

class MiningPolicyControl {
  private:
    std::set<std::string> allowedAddresses;
    boost::shared_mutex addrLock;
    boost::filesystem::path pathAddresses;
    filewatch::FileWatch<std::string>* pWatch;

    void initAddresses(void) {
        addAddress("git che");  // markus
        addAddress("DTjNXq6evTztRAdRD6mqfGftLX359fixkQ");  // maxirmx
    }
    void clearAddresses(void) {
        boost::unique_lock<boost::shared_mutex> uniqueLock(addrLock);
        allowedAddresses.clear();
    }

  public:
    MiningPolicyControl(void): pathAddresses(GetDataDir() / "miners.dat") {
        try {
            loadFromFile();
            pWatch = new filewatch::FileWatch<std::string>(
	            boost::filesystem::canonical(pathAddresses).string(), 
	            [this](const std::string& path, const filewatch::Event change_type) {
                            const char* eventName = "unknown";
                            switch (change_type) {
		                          case filewatch::Event::added:
                                eventName = "added";
                                break;
    	                        case filewatch::Event::removed:
                                eventName = "removed";
                                break;
                              case filewatch::Event::modified:
                                eventName = "modified";
                                break;
                              case filewatch::Event::renamed_old:
                                eventName = "renamed to other name";
                                break;
                              case filewatch::Event::renamed_new:
                                eventName = "renamed from other file";
                                break;
		                        };
                            LogPrintf("MiningPolicyControl: %s has been %s\n", path.c_str(), eventName);
                            loadFromFile();
	            });
        }
        catch (...) {
        }

    }
    ~MiningPolicyControl() {
        if (pWatch) {
            delete pWatch;
        }
    }
    bool addAddress(const std::string& addr) {
        CBitcoinAddress bAddr(addr);
        bool ret = false;
        if (bAddr.IsValid()) {
          LogPrintf("MiningPolicyControl: enabking mining for %s\n", __func__, addr.c_str());
          boost::unique_lock<boost::shared_mutex> uniqueLock(addrLock);
          allowedAddresses.insert(addr);
          ret = true;
        }
        return ret;
    }
    bool isMiningAllowed(const std::string& addr) {
        boost::shared_lock<boost::shared_mutex> lock(addrLock);
        auto search = allowedAddresses.find(addr);
        bool ret  = (search != allowedAddresses.end());
        return ret;   
    }
    void loadFromFile(void) {
        clearAddresses();
        initAddresses(); 
        try {
          std::ifstream infile(pathAddresses.c_str());
          if (infile) {
            std::string addr;
            while (std::getline(infile, addr)) {
              addAddress(addr);
            }
          }
        }
        catch(...) {

        }
    }
};

static MiningPolicyControl* mpControl = NULL;

bool InitMiningPolicyControl(void) {
    mpControl = new MiningPolicyControl();
    return mpControl != NULL;
}

void ShutdownMiningPolicyControl(void) {
   if (mpControl) {
       delete mpControl;
   }
   mpControl = NULL;
}

int IsMiningAllowed(const std::string& addr, std::string& reason) {
  int ret = RPC_NO_ERROR;
  if (!mpControl) {
    reason = std::string("Mining control has not been initialized");
    ret = RPC_MINING_CONTROL_NOT_INITIALIZED;
  }
  if (!mpControl->isMiningAllowed(addr)) {
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
