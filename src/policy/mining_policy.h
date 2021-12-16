// Copyright (c) 2021 CoinGreen Core developers

#pragma once

#include <string>

#include "base58.h"
#include "primitives/transaction.h"
#include "script/standard.h"

bool InitMiningPolicyControl(void);
void ShutdownMiningPolicyControl();
int IsMiningAllowed(const std::string& addr, std::string& reason);
int IsMiningAllowed(const CBitcoinAddress& addr, std::string& reason);
int IsMiningAllowed(const CScript& scrpt, std::string& reason);