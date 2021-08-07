// Copyright (c) 2015 The Dogecoin Core developers
// Copyright (c) 2021 CoinGreen Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "arith_uint256.h"
#include "chainparams.h"
#include "coingreen.h"
#include "test/test_bitcoin.h"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(coingreen_tests, TestingSetup)

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    int nHeight = 1;
    const CChainParams& mainParams = Params(CBaseChainParams::MAIN);
    CAmount nSubsidy = 0;
    CAmount nSum = GENESIS_BLOCK_REWARD;
    uint256 prevHash = uint256S("0");

    int threshold = (INITIAL_SUPPLY-GENESIS_BLOCK_REWARD)/REWARD_PER_BLOCK; 

    for (nHeight = 1; nHeight <= threshold; nHeight++) {
        nSubsidy = GetCoinGreenBlockSubsidy(nHeight, mainParams.GetConsensus(nHeight), prevHash);
        nSum += nSubsidy;        
        BOOST_CHECK(nSubsidy == REWARD_PER_BLOCK * COIN);
    }
    nSubsidy = GetCoinGreenBlockSubsidy(nHeight, mainParams.GetConsensus(nHeight), prevHash);
    BOOST_CHECK(nSubsidy == (INITIAL_SUPPLY - threshold*REWARD_PER_BLOCK - GENESIS_BLOCK_REWARD) * COIN);
    nSum += nSubsidy;

    nSubsidy = GetCoinGreenBlockSubsidy(nHeight+1, mainParams.GetConsensus(nHeight), prevHash);
    BOOST_CHECK(nSubsidy == 0);

    nSubsidy = GetCoinGreenBlockSubsidy(nHeight+100, mainParams.GetConsensus(nHeight), prevHash);
    BOOST_CHECK(nSubsidy == 0);

    
    BOOST_CHECK(nSum/COIN == (INITIAL_SUPPLY- GENESIS_BLOCK_REWARD));

}

BOOST_AUTO_TEST_SUITE_END()
