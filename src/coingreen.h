// Copyright (c)      2015 The Dogecoin Core developers
// Copyright (c) 2021-2022 The CoinGreen Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "amount.h"
#include "chain.h"
#include "chainparams.h"



// Initial supply - 43,000,000
// Mining reward per block 23

const CAmount INITIAL_SUPPLY          = 43000000;
const CAmount FOUNDER_MINT            = 10000000;
const CAmount REWARD_PER_BLOCK        = 23;
const CAmount GENESIS_BLOCK_REWARD    = 88;


bool AllowDigishieldMinDifficultyForBlock(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params);
CAmount GetCoinGreenBlockSubsidy(uint32_t nHeight, const Consensus::Params& consensusParams, uint256 prevHash);
unsigned int CalculateCoinGreenNextWorkRequired(const CBlockIndex* pindexLast, int64_t nLastRetargetTime, const Consensus::Params& params);

/**
 * Check proof-of-work of a block header, taking auxpow into account.
 * @param block The block header.
 * @param params Consensus parameters.
 * @return True iff the PoW is correct.
 */
bool CheckAuxPowProofOfWork(const CBlockHeader& block, const Consensus::Params& params);

CAmount GetCoinGreenMinRelayFee(const CTransaction& tx, unsigned int nBytes, bool fAllowFree);
CAmount GetCoinGreenDustFee(const std::vector<CTxOut> &vout, CFeeRate &baseFeeRate);
