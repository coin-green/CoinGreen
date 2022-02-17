// Copyright (c) 2015 The Dogecoin Core developers
// Copyright (c) 2021-2022 The CoinGreen Core developers
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
    uint32_t nHeight = 1;
    const CChainParams& mainParams = Params(CBaseChainParams::MAIN);
    CAmount subsidy = 0;
    CAmount sum = GENESIS_BLOCK_REWARD * COIN;
    uint256 prevHash = uint256S("0");

    uint32_t nThreshold = (INITIAL_SUPPLY - GENESIS_BLOCK_REWARD - FOUNDER_MINT)/REWARD_PER_BLOCK + 1;

    for (nHeight = 1; nHeight <= nThreshold; nHeight++) {
        subsidy = GetCoinGreenBlockSubsidy(nHeight, mainParams.GetConsensus(nHeight), prevHash);
        sum += subsidy;
        BOOST_CHECK(subsidy == ((nHeight == mainParams.GetConsensus(nHeight).nFounderMintHeight ? FOUNDER_MINT : REWARD_PER_BLOCK) * COIN));
    }
    subsidy = GetCoinGreenBlockSubsidy(nHeight, mainParams.GetConsensus(nHeight), prevHash);
    CAmount expected = (INITIAL_SUPPLY - FOUNDER_MINT - (nThreshold-1)*REWARD_PER_BLOCK - GENESIS_BLOCK_REWARD);
    BOOST_CHECK(subsidy == expected * COIN);
    sum += subsidy;

    subsidy = GetCoinGreenBlockSubsidy(nHeight+1, mainParams.GetConsensus(nHeight), prevHash);
    BOOST_CHECK(subsidy == 0);

    subsidy = GetCoinGreenBlockSubsidy(nHeight+100, mainParams.GetConsensus(nHeight), prevHash);
    BOOST_CHECK(subsidy == 0);

    BOOST_CHECK(sum/COIN == INITIAL_SUPPLY);

}

BOOST_AUTO_TEST_SUITE_END()
