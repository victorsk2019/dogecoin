// Copyright (c) 2012-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

<<<<<<< HEAD
#include "txmempool.h"
#include "wallet/wallet.h"
=======
#include <wallet/wallet.h>
>>>>>>> 1.21-dev

#include <future>
#include <memory>
#include <stdint.h>
#include <vector>

<<<<<<< HEAD
#include "rpc/server.h"
#include "test/test_bitcoin.h"
#include "validation.h"
#include "wallet/test/wallet_test_fixture.h"
=======
#include <interfaces/chain.h>
#include <node/context.h>
#include <policy/policy.h>
#include <rpc/server.h>
#include <test/util/logging.h>
#include <test/util/setup_common.h>
#include <util/ref.h>
#include <util/translation.h>
#include <validation.h>
#include <wallet/coincontrol.h>
#include <wallet/test/wallet_test_fixture.h>
>>>>>>> 1.21-dev

#include <boost/test/unit_test.hpp>
#include <univalue.h>
<<<<<<< HEAD

extern UniValue importmulti(const JSONRPCRequest& request);
extern UniValue dumpwallet(const JSONRPCRequest& request);
extern UniValue importwallet(const JSONRPCRequest& request);
=======
>>>>>>> 1.21-dev

RPCHelpMan importmulti();
RPCHelpMan dumpwallet();
RPCHelpMan importwallet();

// Ensure that fee levels defined in the wallet are at least as high
// as the default levels for node policy.
static_assert(DEFAULT_TRANSACTION_MINFEE >= DEFAULT_MIN_RELAY_TX_FEE, "wallet minimum fee is smaller than default relay fee");
static_assert(WALLET_INCREMENTAL_RELAY_FEE >= DEFAULT_INCREMENTAL_RELAY_FEE, "wallet incremental fee is smaller than default incremental relay fee");

extern CAmount nDustLimit;

BOOST_FIXTURE_TEST_SUITE(wallet_tests, WalletTestingSetup)

static std::shared_ptr<CWallet> TestLoadWallet(interfaces::Chain& chain)
{
    DatabaseOptions options;
    DatabaseStatus status;
    bilingual_str error;
    std::vector<bilingual_str> warnings;
    auto database = MakeWalletDatabase("", options, status, error);
    auto wallet = CWallet::Create(chain, "", std::move(database), options.create_flags, error, warnings);
    wallet->postInitProcess();
    return wallet;
}

static void TestUnloadWallet(std::shared_ptr<CWallet>&& wallet)
{
    SyncWithValidationInterfaceQueue();
    wallet->m_chain_notifications_handler.reset();
    UnloadWallet(std::move(wallet));
}

static CMutableTransaction TestSimpleSpend(const CTransaction& from, uint32_t index, const CKey& key, const CScript& pubkey)
{
    CMutableTransaction mtx;
    mtx.vout.push_back({from.vout[index].nValue - DEFAULT_TRANSACTION_MAXFEE, pubkey});
    mtx.vin.push_back({CTxIn{from.GetHash(), index}});
    FillableSigningProvider keystore;
    keystore.AddKey(key);
    std::map<COutPoint, Coin> coins;
    coins[mtx.vin[0].prevout].out = from.vout[index];
    std::map<int, std::string> input_errors;
    BOOST_CHECK(SignTransaction(mtx, &keystore, coins, SIGHASH_ALL, input_errors));
    return mtx;
}

static void AddKey(CWallet& wallet, const CKey& key)
{
    auto spk_man = wallet.GetOrCreateLegacyScriptPubKeyMan();
    LOCK2(wallet.cs_wallet, spk_man->cs_KeyStore);
    spk_man->AddKeyPubKey(key, key.GetPubKey());
}

BOOST_FIXTURE_TEST_CASE(scan_for_wallet_transactions, TestChain240Setup)
{
    // Cap last block file size, and mine new block in a new block file.
    CBlockIndex* oldTip = ::ChainActive().Tip();
    GetBlockFileInfo(oldTip->GetBlockPos().nFile)->nSize = MAX_BLOCKFILE_SIZE;
    CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey()));
    CBlockIndex* newTip = ::ChainActive().Tip();

    NodeContext node;
    auto chain = interfaces::MakeChain(node);

    // Verify ScanForWalletTransactions fails to read an unknown start block.
    {
<<<<<<< HEAD
        wtx->fDebitCached = true;
        wtx->nDebitCached = 1;
    }
    COutput output(wtx.get(), nInput, nAge, true, true);
    vCoins.push_back(output);
    wtxn.emplace_back(std::move(wtx));
}

static void empty_wallet(void)
{
    vCoins.clear();
    wtxn.clear();
}

static bool equal_sets(CoinSet a, CoinSet b)
{
    pair<CoinSet::iterator, CoinSet::iterator> ret = mismatch(a.begin(), a.end(), b.begin());
    return ret.first == a.end() && ret.second == b.end();
}

BOOST_AUTO_TEST_CASE(coin_selection_tests)
{
    CoinSet setCoinsRet, setCoinsRet2;
    CAmount nValueRet;

    LOCK(wallet.cs_wallet);

    // test multiple times to allow for differences in the shuffle order
    for (int i = 0; i < RUN_TESTS; i++)
    {
        empty_wallet();

        // with an empty wallet we can't even pay one coin
        BOOST_CHECK(!wallet.SelectCoinsMinConf( 1 * COIN, 1, 6, 0, vCoins, setCoinsRet, nValueRet));

        add_coin(10*COIN, 4);        // add a new 10 coin output

        // with only a new 1 coin output, we still can't find a mature 10 coin output
        BOOST_CHECK(!wallet.SelectCoinsMinConf( 10 * COIN, 1, 6, 0, vCoins, setCoinsRet, nValueRet));

        // but we can find a new 10 coin output
        BOOST_CHECK( wallet.SelectCoinsMinConf( 10 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 10 * COIN);

        add_coin(20*COIN);           // add a mature 20 coin output

        // we can't make 30 coins of mature outputs
        BOOST_CHECK(!wallet.SelectCoinsMinConf( 30 * COIN, 1, 6, 0, vCoins, setCoinsRet, nValueRet));

        // we can make 30 coin of new outputs
        BOOST_CHECK( wallet.SelectCoinsMinConf( 30 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 30 * COIN);

        add_coin(50*COIN);           // add a mature 50 coin output,
        add_coin(100*COIN, 3, true); // a new 100 coin output sent from one of our own addresses
        add_coin(200*COIN);          // and a mature 200 coin output

        // now we have new: 10+100=110 (of which 100 was self-sent), and mature: 20+50+200=270.  total = 380

        // we can't make 380 coins only if we disallow new output:
        BOOST_CHECK(!wallet.SelectCoinsMinConf(380 * COIN, 1, 6, 0, vCoins, setCoinsRet, nValueRet));
        // we can't even make 370 coins if we don't allow new output even if they're from us
        BOOST_CHECK(!wallet.SelectCoinsMinConf(380 * COIN, 6, 6, 0, vCoins, setCoinsRet, nValueRet));
        // but we can make 370 coins if we accept new output from ourself
        BOOST_CHECK( wallet.SelectCoinsMinConf(370 * COIN, 1, 6, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 370 * COIN);
        // and we can make 380 coins if we accept all new output
        BOOST_CHECK( wallet.SelectCoinsMinConf(380 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 380 * COIN);

        // try making 340 coins from 10,20,50,100,200 - we can't do it exactly
        BOOST_CHECK( wallet.SelectCoinsMinConf(340 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 350 * COIN);       // but 350 coins is closest
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 3U);     // the best should be 200+100+50.  it's incredibly unlikely the 10 or 20 got included (but possible)

        // when we try making 70 coins, the smaller outputs (10,20,50) are enough.  We should see just 20+50
        BOOST_CHECK( wallet.SelectCoinsMinConf( 70 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 70 * COIN);
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 2U);

        // when we try making 80 coins, the smaller outputs (10,20,50) are exactly enough.
        BOOST_CHECK( wallet.SelectCoinsMinConf( 80 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK(nValueRet == 80 * COIN);
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 3U);

        // when we try making 90 coins, no subset of smaller outputs is enough, and we get the next bigger output (100)
        BOOST_CHECK( wallet.SelectCoinsMinConf( 90 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 100 * COIN);
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 1U);

        // now clear out the wallet and start again to test choosing between subsets of smaller coins and the next biggest coin
        empty_wallet();

        add_coin( 60*COIN);
        add_coin( 70*COIN);
        add_coin( 80*COIN);
        add_coin(200*COIN);
        add_coin(300*COIN); // now we have 60+70+80+200+300 = 710 coins total

        // check that we have 710 and not 711
        BOOST_CHECK( wallet.SelectCoinsMinConf(710 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK(!wallet.SelectCoinsMinConf(711 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));

        // now try making 160 coins.  the best smaller outputs can do is 60+70+80 = 210; not as good at the next biggest output, 200
        BOOST_CHECK( wallet.SelectCoinsMinConf(160 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 200 * COIN); // we should get 200 in one output
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 1U);

        add_coin( 50*COIN); // now we have 50+60+70+80+200+300 = 750 coins total

        // now if we try making 160 coins again, the smaller outputs can make 50+60+70 = 18 coins, better than the next biggest output, 200
        BOOST_CHECK( wallet.SelectCoinsMinConf(160 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 180 * COIN); // we should get 180 in 3 outputs
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 3U);

        add_coin( 180*COIN); // now we have 50+60+70+80+180+200+300

        // and now if we try making 16 coins again, the smaller outputs can make 50+60+70 = 180 coins, the same as the next biggest output, 180
        BOOST_CHECK( wallet.SelectCoinsMinConf(160 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 180 * COIN);  // we should get 180 in 1 output
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 1U); // because in the event of a tie, the biggest output wins

        // now try making 110 coins.  we should get 50+60
        BOOST_CHECK( wallet.SelectCoinsMinConf(110 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 110 * COIN);
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 2U);

        // check that the smallest bigger output is used
        add_coin( 1000*COIN);
        add_coin( 2000*COIN);
        add_coin( 3000*COIN);
        add_coin( 4000*COIN); // now we have 50+60+70+80+180+200+300+1000+2000+3000+4000 = 10940 coins
        BOOST_CHECK( wallet.SelectCoinsMinConf(950 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 1000 * COIN);  // we should get 1000 coins in 1 output
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 1U);

        BOOST_CHECK( wallet.SelectCoinsMinConf(1950 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 2000 * COIN);  // we should get 2000 coins in 1 output
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 1U);

        // empty the wallet and start again, now with fractions of a coin, to test small change avoidance

        empty_wallet();
        add_coin(CWallet::GetMinChange() * 1 / 10);
        add_coin(CWallet::GetMinChange() * 2 / 10);
        add_coin(CWallet::GetMinChange() * 3 / 10);
        add_coin(CWallet::GetMinChange() * 4 / 10);
        add_coin(CWallet::GetMinChange() * 5 / 10);

        // try making 1 * GetMinChange() from the 1.5 * GetMinChange()
        // we'll get change smaller than GetMinChange() whatever happens, so can expect GetMinChange() exactly
        BOOST_CHECK( wallet.SelectCoinsMinConf(CWallet::GetMinChange(), 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, CWallet::GetMinChange());

        // but if we add a bigger output, small change is avoided
        add_coin(1111*CWallet::GetMinChange());

        // try making 1 from 0.1 + 0.2 + 0.3 + 0.4 + 0.5 + 1111 = 1112.5
        BOOST_CHECK( wallet.SelectCoinsMinConf(1 * CWallet::GetMinChange(), 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 1 * CWallet::GetMinChange()); // we should get the exact amount

        // if we add more small output:
        add_coin(CWallet::GetMinChange() * 6 / 10);
        add_coin(CWallet::GetMinChange() * 7 / 10);

        // and try again to make 1.0 * GetMinChange()
        BOOST_CHECK( wallet.SelectCoinsMinConf(1 * CWallet::GetMinChange(), 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 1 * CWallet::GetMinChange()); // we should get the exact amount

        // run the 'mtgox' test (see http://blockexplorer.com/tx/29a3efd3ef04f9153d47a990bd7b048a4b2d213daaa5fb8ed670fb85f13bdbcf)
        // they tried to consolidate 10 50k outputs into one 500k output, and ended up with 50k in change
        empty_wallet();
        for (int j = 0; j < 20; j++)
            add_coin(50000 * COIN);

        BOOST_CHECK( wallet.SelectCoinsMinConf(500000 * COIN, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 500000 * COIN); // we should get the exact amount
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 10U); // in ten outputs

        // if there's not enough in the smaller coins to make at least 1 * GetMinChange() change (0.5+0.6+0.7 < 1.0+1.0),
        // we need to try finding an exact subset anyway

        // sometimes it will fail, and so we use the next biggest output:
        empty_wallet();
        add_coin(CWallet::GetMinChange() * 5 / 10);
        add_coin(CWallet::GetMinChange() * 6 / 10);
        add_coin(CWallet::GetMinChange() * 7 / 10);
        add_coin(1111 * CWallet::GetMinChange());
        BOOST_CHECK( wallet.SelectCoinsMinConf(1 * CWallet::GetMinChange(), 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 1111 * CWallet::GetMinChange()); // we get the bigger output
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 1U);

        // but sometimes it's possible, and we use an exact subset (0.4 + 0.6 = 1.0)
        empty_wallet();
        add_coin(CWallet::GetMinChange() * 4 / 10);
        add_coin(CWallet::GetMinChange() * 6 / 10);
        add_coin(CWallet::GetMinChange() * 8 / 10);
        add_coin(1111 * CWallet::GetMinChange());
        BOOST_CHECK( wallet.SelectCoinsMinConf(CWallet::GetMinChange(), 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, CWallet::GetMinChange());   // we should get the exact amount
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 2U); // in two outputs 0.4+0.6

        // test avoiding small change
        empty_wallet();
        add_coin(CWallet::GetMinChange() * 5 / 100);
        add_coin(CWallet::GetMinChange() * 1);
        add_coin(CWallet::GetMinChange() * 100);

        // trying to make 100.01 from these three outputs
        BOOST_CHECK(wallet.SelectCoinsMinConf(CWallet::GetMinChange() * 10001 / 100, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, CWallet::GetMinChange() * 10105 / 100); // we should get all outputs
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 3U);

        // but if we try to make 99.9, we should take the bigger of the two small outputs to avoid small change
        BOOST_CHECK(wallet.SelectCoinsMinConf(CWallet::GetMinChange() * 9990 / 100, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
        BOOST_CHECK_EQUAL(nValueRet, 101 * CWallet::GetMinChange());
        BOOST_CHECK_EQUAL(setCoinsRet.size(), 2U);

        // test with many inputs
        for (CAmount amt=15*CENT; amt < 10000 * COIN; amt*=10) {
             empty_wallet();
             // Create 676 inputs (=  (old MAX_STANDARD_TX_SIZE == 100000)  / 148 bytes per input)
             for (uint16_t j = 0; j < 676; j++)
                 add_coin(amt);
             BOOST_CHECK(wallet.SelectCoinsMinConf(20*CENT, 1, 1, 0, vCoins, setCoinsRet, nValueRet));
             if (amt - 20*CENT < CWallet::GetMinChange()) {
                 // needs more than one input:
                 uint16_t returnSize = std::ceil((20.0 * CENT + CWallet::GetMinChange())/amt);
                 CAmount returnValue = amt * returnSize;
                 BOOST_CHECK_EQUAL(nValueRet, returnValue);
                 BOOST_CHECK_EQUAL(setCoinsRet.size(), returnSize);
             } else {
                 // one input is sufficient:
                 BOOST_CHECK_EQUAL(nValueRet, amt);
                 BOOST_CHECK_EQUAL(setCoinsRet.size(), 1U);
             }
        }

        // test randomness
        {
            empty_wallet();
            for (int i2 = 0; i2 < 100; i2++)
                add_coin(COIN);

            // picking 50 from 100 outputs doesn't depend on the shuffle,
            // but does depend on randomness in the stochastic approximation code
            BOOST_CHECK(wallet.SelectCoinsMinConf(50 * COIN, 1, 6, 0, vCoins, setCoinsRet , nValueRet));
            BOOST_CHECK(wallet.SelectCoinsMinConf(50 * COIN, 1, 6, 0, vCoins, setCoinsRet2, nValueRet));
            BOOST_CHECK(!equal_sets(setCoinsRet, setCoinsRet2));

            int fails = 0;
            for (int j = 0; j < RANDOM_REPEATS; j++)
            {
                // selecting 1 from 100 identical outputs depends on the shuffle; this test will fail 1% of the time
                // run the test RANDOM_REPEATS times and only complain if all of them fail
                BOOST_CHECK(wallet.SelectCoinsMinConf(COIN, 1, 6, 0, vCoins, setCoinsRet , nValueRet));
                BOOST_CHECK(wallet.SelectCoinsMinConf(COIN, 1, 6, 0, vCoins, setCoinsRet2, nValueRet));
                if (equal_sets(setCoinsRet, setCoinsRet2))
                    fails++;
            }
            BOOST_CHECK_NE(fails, RANDOM_REPEATS);

            // add 75 coins in small change.  not enough to make 90 coins,
            // then try making 90 coins.  there are multiple competing "smallest bigger" outputs,
            // one of which should be picked at random
            add_coin(5 * COIN);
            add_coin(10 * COIN);
            add_coin(15 * COIN);
            add_coin(20 * COIN);
            add_coin(25 * COIN);

            fails = 0;
            for (int j = 0; j < RANDOM_REPEATS; j++)
            {
                // selecting 1 from 100 identical outputs depends on the shuffle; this test will fail 1% of the time
                // run the test RANDOM_REPEATS times and only complain if all of them fail
                BOOST_CHECK(wallet.SelectCoinsMinConf(90*COIN, 1, 6, 0, vCoins, setCoinsRet , nValueRet));
                BOOST_CHECK(wallet.SelectCoinsMinConf(90*COIN, 1, 6, 0, vCoins, setCoinsRet2, nValueRet));
                if (equal_sets(setCoinsRet, setCoinsRet2))
                    fails++;
            }
            BOOST_CHECK_NE(fails, RANDOM_REPEATS);
=======
        CWallet wallet(chain.get(), "", CreateDummyWalletDatabase());
        {
            LOCK(wallet.cs_wallet);
            wallet.SetLastBlockProcessed(::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash());
>>>>>>> 1.21-dev
        }
        AddKey(wallet, coinbaseKey);
        WalletRescanReserver reserver(wallet);
        reserver.reserve();
        CWallet::ScanResult result = wallet.ScanForWalletTransactions({} /* start_block */, 0 /* start_height */, {} /* max_height */, reserver, false /* update */);
        BOOST_CHECK_EQUAL(result.status, CWallet::ScanResult::FAILURE);
        BOOST_CHECK(result.last_failed_block.IsNull());
        BOOST_CHECK(result.last_scanned_block.IsNull());
        BOOST_CHECK(!result.last_scanned_height);
        BOOST_CHECK_EQUAL(wallet.GetBalance().m_mine_immature, 0);
    }

    // Verify ScanForWalletTransactions picks up transactions in both the old
    // and new block files.
    {
        CWallet wallet(chain.get(), "", CreateDummyWalletDatabase());
        {
            LOCK(wallet.cs_wallet);
            wallet.SetLastBlockProcessed(::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash());
        }
        AddKey(wallet, coinbaseKey);
        WalletRescanReserver reserver(wallet);
        reserver.reserve();
        CWallet::ScanResult result = wallet.ScanForWalletTransactions(oldTip->GetBlockHash(), oldTip->nHeight, {} /* max_height */, reserver, false /* update */);
        BOOST_CHECK_EQUAL(result.status, CWallet::ScanResult::SUCCESS);
        BOOST_CHECK(result.last_failed_block.IsNull());
        BOOST_CHECK_EQUAL(result.last_scanned_block, newTip->GetBlockHash());
        BOOST_CHECK_EQUAL(*result.last_scanned_height, newTip->nHeight);
        // Dogecoin: Immature balance varies due to random rewards
        BOOST_CHECK(wallet.GetBalance().m_mine_immature <= (240 * 1000000 * COIN));
    }

    // Prune the older block file.
    {
        LOCK(cs_main);
        Assert(m_node.chainman)->m_blockman.PruneOneBlockFile(oldTip->GetBlockPos().nFile);
    }
    UnlinkPrunedFiles({oldTip->GetBlockPos().nFile});

    // Verify ScanForWalletTransactions only picks transactions in the new block
    // file.
    {
        CWallet wallet(chain.get(), "", CreateDummyWalletDatabase());
        {
            LOCK(wallet.cs_wallet);
            wallet.SetLastBlockProcessed(::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash());
        }
        AddKey(wallet, coinbaseKey);
        WalletRescanReserver reserver(wallet);
        reserver.reserve();
        CWallet::ScanResult result = wallet.ScanForWalletTransactions(oldTip->GetBlockHash(), oldTip->nHeight, {} /* max_height */, reserver, false /* update */);
        BOOST_CHECK_EQUAL(result.status, CWallet::ScanResult::FAILURE);
        BOOST_CHECK_EQUAL(result.last_failed_block, oldTip->GetBlockHash());
        BOOST_CHECK_EQUAL(result.last_scanned_block, newTip->GetBlockHash());
        BOOST_CHECK_EQUAL(*result.last_scanned_height, newTip->nHeight);
	// Dogecoin: Scaled up by 10,000
        BOOST_CHECK_EQUAL(wallet.GetBalance().m_mine_immature, 250000 * COIN);
    }

    // Prune the remaining block file.
    {
        LOCK(cs_main);
        Assert(m_node.chainman)->m_blockman.PruneOneBlockFile(newTip->GetBlockPos().nFile);
    }
    UnlinkPrunedFiles({newTip->GetBlockPos().nFile});

    // Verify ScanForWalletTransactions scans no blocks.
    {
        CWallet wallet(chain.get(), "", CreateDummyWalletDatabase());
        {
            LOCK(wallet.cs_wallet);
            wallet.SetLastBlockProcessed(::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash());
        }
        AddKey(wallet, coinbaseKey);
        WalletRescanReserver reserver(wallet);
        reserver.reserve();
        CWallet::ScanResult result = wallet.ScanForWalletTransactions(oldTip->GetBlockHash(), oldTip->nHeight, {} /* max_height */, reserver, false /* update */);
        BOOST_CHECK_EQUAL(result.status, CWallet::ScanResult::FAILURE);
        BOOST_CHECK_EQUAL(result.last_failed_block, newTip->GetBlockHash());
        BOOST_CHECK(result.last_scanned_block.IsNull());
        BOOST_CHECK(!result.last_scanned_height);
        BOOST_CHECK_EQUAL(wallet.GetBalance().m_mine_immature, 0);
    }
}

BOOST_FIXTURE_TEST_CASE(importmulti_rescan, TestChain240Setup)
{
    // Cap last block file size, and mine new block in a new block file.
    CBlockIndex* oldTip = ::ChainActive().Tip();
    GetBlockFileInfo(oldTip->GetBlockPos().nFile)->nSize = MAX_BLOCKFILE_SIZE;
    CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey()));
    CBlockIndex* newTip = ::ChainActive().Tip();

    NodeContext node;
    auto chain = interfaces::MakeChain(node);

    // Prune the older block file.
    {
        LOCK(cs_main);
        Assert(m_node.chainman)->m_blockman.PruneOneBlockFile(oldTip->GetBlockPos().nFile);
    }
    UnlinkPrunedFiles({oldTip->GetBlockPos().nFile});

    // Verify importmulti RPC returns failure for a key whose creation time is
    // before the missing block, and success for a key whose creation time is
    // after.
    {
        std::shared_ptr<CWallet> wallet = std::make_shared<CWallet>(chain.get(), "", CreateDummyWalletDatabase());
        wallet->SetupLegacyScriptPubKeyMan();
        WITH_LOCK(wallet->cs_wallet, wallet->SetLastBlockProcessed(newTip->nHeight, newTip->GetBlockHash()));
        AddWallet(wallet);
        UniValue keys;
        keys.setArray();
        UniValue key;
        key.setObject();
        key.pushKV("scriptPubKey", HexStr(GetScriptForRawPubKey(coinbaseKey.GetPubKey())));
        key.pushKV("timestamp", 0);
        key.pushKV("internal", UniValue(true));
        keys.push_back(key);
        key.clear();
        key.setObject();
        CKey futureKey;
        futureKey.MakeNewKey(true);
        key.pushKV("scriptPubKey", HexStr(GetScriptForRawPubKey(futureKey.GetPubKey())));
        key.pushKV("timestamp", newTip->GetBlockTimeMax() + TIMESTAMP_WINDOW + 1);
        key.pushKV("internal", UniValue(true));
        keys.push_back(key);
        util::Ref context;
        JSONRPCRequest request(context);
        request.params.setArray();
        request.params.push_back(keys);

        UniValue response = importmulti().HandleRequest(request);
        BOOST_CHECK_EQUAL(response.write(),
            strprintf("[{\"success\":false,\"error\":{\"code\":-1,\"message\":\"Rescan failed for key with creation "
                      "timestamp %d. There was an error reading a block from time %d, which is after or within %d "
                      "seconds of key creation, and could contain transactions pertaining to the key. As a result, "
                      "transactions and coins using this key may not appear in the wallet. This error could be caused "
                      "by pruning or data corruption (see bitcoind log for details) and could be dealt with by "
                      "downloading and rescanning the relevant blocks (see -reindex and -rescan "
                      "options).\"}},{\"success\":true}]",
                              0, oldTip->GetBlockTimeMax(), TIMESTAMP_WINDOW));
        RemoveWallet(wallet, nullopt);
    }
}

// Verify importwallet RPC starts rescan at earliest block with timestamp
// greater or equal than key birthday. Previously there was a bug where
// importwallet RPC would start the scan at the latest block with timestamp less
// than or equal to key birthday.
BOOST_FIXTURE_TEST_CASE(importwallet_rescan, TestChain240Setup)
{
    // Create two blocks with same timestamp to verify that importwallet rescan
    // will pick up both blocks, not just the first.
    const int64_t BLOCK_TIME = ::ChainActive().Tip()->GetBlockTimeMax() + 5;
    SetMockTime(BLOCK_TIME);
    m_coinbase_txns.emplace_back(CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);
    m_coinbase_txns.emplace_back(CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);

    // Set key birthday to block time increased by the timestamp window, so
    // rescan will start at the block time.
    const int64_t KEY_TIME = BLOCK_TIME + TIMESTAMP_WINDOW;
    SetMockTime(KEY_TIME);
    m_coinbase_txns.emplace_back(CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);

    NodeContext node;
    auto chain = interfaces::MakeChain(node);

    std::string backup_file = (GetDataDir() / "wallet.backup").string();

    // Import key into wallet and call dumpwallet to create backup file.
    {
        std::shared_ptr<CWallet> wallet = std::make_shared<CWallet>(chain.get(), "", CreateDummyWalletDatabase());
        {
            auto spk_man = wallet->GetOrCreateLegacyScriptPubKeyMan();
            LOCK2(wallet->cs_wallet, spk_man->cs_KeyStore);
            spk_man->mapKeyMetadata[coinbaseKey.GetPubKey().GetID()].nCreateTime = KEY_TIME;
            spk_man->AddKeyPubKey(coinbaseKey, coinbaseKey.GetPubKey());

            AddWallet(wallet);
            wallet->SetLastBlockProcessed(::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash());
        }
        util::Ref context;
        JSONRPCRequest request(context);
        request.params.setArray();
        request.params.push_back(backup_file);

        ::dumpwallet().HandleRequest(request);
        RemoveWallet(wallet, nullopt);
    }

    // Call importwallet RPC and verify all blocks with timestamps >= BLOCK_TIME
    // were scanned, and no prior blocks were scanned.
    {
        std::shared_ptr<CWallet> wallet = std::make_shared<CWallet>(chain.get(), "", CreateDummyWalletDatabase());
        LOCK(wallet->cs_wallet);
        wallet->SetupLegacyScriptPubKeyMan();

        util::Ref context;
        JSONRPCRequest request(context);
        request.params.setArray();
        request.params.push_back(backup_file);
        AddWallet(wallet);
        wallet->SetLastBlockProcessed(::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash());
        ::importwallet().HandleRequest(request);
        RemoveWallet(wallet, nullopt);

        BOOST_CHECK_EQUAL(wallet->mapWallet.size(), 3U);
        BOOST_CHECK_EQUAL(m_coinbase_txns.size(), 243U);
        for (size_t i = 0; i < m_coinbase_txns.size(); ++i) {
            bool found = wallet->GetWalletTx(m_coinbase_txns[i]->GetHash());
            bool expected = i >= 240;
            BOOST_CHECK_EQUAL(found, expected);
        }
    }

    SetMockTime(0);
}

// Check that GetImmatureCredit() returns a newly calculated value instead of
// the cached value after a MarkDirty() call.
//
// This is a regression test written to verify a bugfix for the immature credit
// function. Similar tests probably should be written for the other credit and
// debit functions.
BOOST_FIXTURE_TEST_CASE(coin_mark_dirty_immature_credit, TestChain240Setup)
{
    NodeContext node;
    auto chain = interfaces::MakeChain(node);

    CWallet wallet(chain.get(), "", CreateDummyWalletDatabase());
    auto spk_man = wallet.GetOrCreateLegacyScriptPubKeyMan();
    CWalletTx wtx(&wallet, m_coinbase_txns.back());

    LOCK2(wallet.cs_wallet, spk_man->cs_KeyStore);
    wallet.SetLastBlockProcessed(::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash());

    CWalletTx::Confirmation confirm(CWalletTx::Status::CONFIRMED, ::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash(), 0);
    wtx.m_confirm = confirm;

    // Call GetImmatureCredit() once before adding the key to the wallet to
    // cache the current immature credit amount, which is 0.
    BOOST_CHECK_EQUAL(wtx.GetImmatureCredit(), 0);

    // Invalidate the cached value, add the key, and make sure a new immature
    // credit amount is calculated.
    wtx.MarkDirty();
    BOOST_CHECK(spk_man->AddKeyPubKey(coinbaseKey, coinbaseKey.GetPubKey()));
    // Dogecoin: Scaled up by 10,000
    BOOST_CHECK_EQUAL(wtx.GetImmatureCredit(), 250000*COIN);
}

static int64_t AddTx(ChainstateManager& chainman, CWallet& wallet, uint32_t lockTime, int64_t mockTime, int64_t blockTime)
{
    CMutableTransaction tx;
    CWalletTx::Confirmation confirm;
    tx.nLockTime = lockTime;
    SetMockTime(mockTime);
    CBlockIndex* block = nullptr;
    if (blockTime > 0) {
        LOCK(cs_main);
        auto inserted = chainman.BlockIndex().emplace(GetRandHash(), new CBlockIndex);
        assert(inserted.second);
        const uint256& hash = inserted.first->first;
        block = inserted.first->second;
        block->nTime = blockTime;
        block->phashBlock = &hash;
        confirm = {CWalletTx::Status::CONFIRMED, block->nHeight, hash, 0};
    }

    // If transaction is already in map, to avoid inconsistencies, unconfirmation
    // is needed before confirm again with different block.
    return wallet.AddToWallet(MakeTransactionRef(tx), confirm, [&](CWalletTx& wtx, bool /* new_tx */) {
        wtx.setUnconfirmed();
        return true;
    })->nTimeSmart;
}

// Simple test to verify assignment of CWalletTx::nSmartTime value. Could be
// expanded to cover more corner cases of smart time logic.
BOOST_AUTO_TEST_CASE(ComputeTimeSmart)
{
    // New transaction should use clock time if lower than block time.
    BOOST_CHECK_EQUAL(AddTx(*m_node.chainman, m_wallet, 1, 100, 120), 100);

    // Test that updating existing transaction does not change smart time.
    BOOST_CHECK_EQUAL(AddTx(*m_node.chainman, m_wallet, 1, 200, 220), 100);

    // New transaction should use clock time if there's no block time.
    BOOST_CHECK_EQUAL(AddTx(*m_node.chainman, m_wallet, 2, 300, 0), 300);

    // New transaction should use block time if lower than clock time.
    BOOST_CHECK_EQUAL(AddTx(*m_node.chainman, m_wallet, 3, 420, 400), 400);

    // New transaction should use latest entry time if higher than
    // min(block time, clock time).
    BOOST_CHECK_EQUAL(AddTx(*m_node.chainman, m_wallet, 4, 500, 390), 400);

    // If there are future entries, new transaction should use time of the
    // newest entry that is no more than 300 seconds ahead of the clock time.
    BOOST_CHECK_EQUAL(AddTx(*m_node.chainman, m_wallet, 5, 50, 600), 300);

    // Reset mock time for other tests.
    SetMockTime(0);
}

BOOST_AUTO_TEST_CASE(LoadReceiveRequests)
{
    CTxDestination dest = PKHash();
    LOCK(m_wallet.cs_wallet);
    WalletBatch batch{m_wallet.GetDatabase()};
    m_wallet.AddDestData(batch, dest, "misc", "val_misc");
    m_wallet.AddDestData(batch, dest, "rr0", "val_rr0");
    m_wallet.AddDestData(batch, dest, "rr1", "val_rr1");

    auto values = m_wallet.GetDestValues("rr");
    BOOST_CHECK_EQUAL(values.size(), 2U);
    BOOST_CHECK_EQUAL(values[0], "val_rr0");
    BOOST_CHECK_EQUAL(values[1], "val_rr1");
}

// Test some watch-only LegacyScriptPubKeyMan methods by the procedure of loading (LoadWatchOnly),
// checking (HaveWatchOnly), getting (GetWatchPubKey) and removing (RemoveWatchOnly) a
// given PubKey, resp. its corresponding P2PK Script. Results of the the impact on
// the address -> PubKey map is dependent on whether the PubKey is a point on the curve
static void TestWatchOnlyPubKey(LegacyScriptPubKeyMan* spk_man, const CPubKey& add_pubkey)
{
    CScript p2pk = GetScriptForRawPubKey(add_pubkey);
    CKeyID add_address = add_pubkey.GetID();
    CPubKey found_pubkey;
    LOCK(spk_man->cs_KeyStore);

    // all Scripts (i.e. also all PubKeys) are added to the general watch-only set
    BOOST_CHECK(!spk_man->HaveWatchOnly(p2pk));
    spk_man->LoadWatchOnly(p2pk);
    BOOST_CHECK(spk_man->HaveWatchOnly(p2pk));

    // only PubKeys on the curve shall be added to the watch-only address -> PubKey map
    bool is_pubkey_fully_valid = add_pubkey.IsFullyValid();
    if (is_pubkey_fully_valid) {
        BOOST_CHECK(spk_man->GetWatchPubKey(add_address, found_pubkey));
        BOOST_CHECK(found_pubkey == add_pubkey);
    } else {
        BOOST_CHECK(!spk_man->GetWatchPubKey(add_address, found_pubkey));
        BOOST_CHECK(found_pubkey == CPubKey()); // passed key is unchanged
    }

    spk_man->RemoveWatchOnly(p2pk);
    BOOST_CHECK(!spk_man->HaveWatchOnly(p2pk));

    if (is_pubkey_fully_valid) {
        BOOST_CHECK(!spk_man->GetWatchPubKey(add_address, found_pubkey));
        BOOST_CHECK(found_pubkey == add_pubkey); // passed key is unchanged
    }
}

// Cryptographically invalidate a PubKey whilst keeping length and first byte
static void PollutePubKey(CPubKey& pubkey)
{
    std::vector<unsigned char> pubkey_raw(pubkey.begin(), pubkey.end());
    std::fill(pubkey_raw.begin()+1, pubkey_raw.end(), 0);
    pubkey = CPubKey(pubkey_raw);
    assert(!pubkey.IsFullyValid());
    assert(pubkey.IsValid());
}

// Test watch-only logic for PubKeys
BOOST_AUTO_TEST_CASE(WatchOnlyPubKeys)
{
    CKey key;
    CPubKey pubkey;
    LegacyScriptPubKeyMan* spk_man = m_wallet.GetOrCreateLegacyScriptPubKeyMan();

    BOOST_CHECK(!spk_man->HaveWatchOnly());

    // uncompressed valid PubKey
    key.MakeNewKey(false);
    pubkey = key.GetPubKey();
    assert(!pubkey.IsCompressed());
    TestWatchOnlyPubKey(spk_man, pubkey);

    // uncompressed cryptographically invalid PubKey
    PollutePubKey(pubkey);
    TestWatchOnlyPubKey(spk_man, pubkey);

    // compressed valid PubKey
    key.MakeNewKey(true);
    pubkey = key.GetPubKey();
    assert(pubkey.IsCompressed());
    TestWatchOnlyPubKey(spk_man, pubkey);

    // compressed cryptographically invalid PubKey
    PollutePubKey(pubkey);
    TestWatchOnlyPubKey(spk_man, pubkey);

    // invalid empty PubKey
    pubkey = CPubKey();
    TestWatchOnlyPubKey(spk_man, pubkey);
}

class ListCoinsTestingSetup : public TestChain240Setup
{
public:
    ListCoinsTestingSetup()
    {
        CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey()));
        wallet = MakeUnique<CWallet>(m_chain.get(), "", CreateMockWalletDatabase());
        {
            LOCK2(wallet->cs_wallet, ::cs_main);
            wallet->SetLastBlockProcessed(::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash());
        }
        bool firstRun;
        wallet->LoadWallet(firstRun);
        AddKey(*wallet, coinbaseKey);
        WalletRescanReserver reserver(*wallet);
        reserver.reserve();
        CWallet::ScanResult result = wallet->ScanForWalletTransactions(::ChainActive().Genesis()->GetBlockHash(), 0 /* start_height */, {} /* max_height */, reserver, false /* update */);
        BOOST_CHECK_EQUAL(result.status, CWallet::ScanResult::SUCCESS);
        BOOST_CHECK_EQUAL(result.last_scanned_block, ::ChainActive().Tip()->GetBlockHash());
        BOOST_CHECK_EQUAL(*result.last_scanned_height, ::ChainActive().Height());
        BOOST_CHECK(result.last_failed_block.IsNull());
    }

    ~ListCoinsTestingSetup()
    {
        wallet.reset();
    }

    CWalletTx& AddTx(CRecipient recipient)
    {
        CTransactionRef tx;
        CAmount fee;
        int changePos = -1;
        bilingual_str error;
        CCoinControl dummy;
        FeeCalculation fee_calc_out;
        {
            BOOST_CHECK(wallet->CreateTransaction({recipient}, tx, fee, changePos, error, dummy, fee_calc_out));
        }
        wallet->CommitTransaction(tx, {}, {});
        CMutableTransaction blocktx;
        {
            LOCK(wallet->cs_wallet);
            blocktx = CMutableTransaction(*wallet->mapWallet.at(tx->GetHash()).tx);
        }
        CreateAndProcessBlock({CMutableTransaction(blocktx)}, GetScriptForRawPubKey(coinbaseKey.GetPubKey()));

        LOCK(wallet->cs_wallet);
        wallet->SetLastBlockProcessed(wallet->GetLastBlockHeight() + 1, ::ChainActive().Tip()->GetBlockHash());
        auto it = wallet->mapWallet.find(tx->GetHash());
        BOOST_CHECK(it != wallet->mapWallet.end());
        CWalletTx::Confirmation confirm(CWalletTx::Status::CONFIRMED, ::ChainActive().Height(), ::ChainActive().Tip()->GetBlockHash(), 1);
        it->second.m_confirm = confirm;
        return it->second;
    }

    std::unique_ptr<interfaces::Chain> m_chain = interfaces::MakeChain(m_node);
    std::unique_ptr<CWallet> wallet;
};

BOOST_FIXTURE_TEST_CASE(ListCoins, ListCoinsTestingSetup)
{
    std::string coinbaseAddress = coinbaseKey.GetPubKey().GetID().ToString();

    // Confirm ListCoins initially returns 1 coin grouped under coinbaseKey
    // address.
    std::map<CTxDestination, std::vector<COutput>> list;
    {
        LOCK(wallet->cs_wallet);
        list = wallet->ListCoins();
    }
    BOOST_CHECK_EQUAL(list.size(), 1U);
    BOOST_CHECK_EQUAL(boost::get<PKHash>(list.begin()->first).ToString(), coinbaseAddress);
    BOOST_CHECK_EQUAL(list.begin()->second.size(), 1U);

    // Check initial balance from one mature coinbase transaction.
    // Dogecoin has random rewards so we can only test against maximum
    BOOST_CHECK(1000000 * COIN >= wallet->GetAvailableBalance());

    // Add a transaction creating a change address, and confirm ListCoins still
    // returns the coin associated with the change address underneath the
    // coinbaseKey pubkey, even though the change address has a different
    // pubkey.
    AddTx(CRecipient{GetScriptForRawPubKey({}), 1 * COIN, false /* subtract fee */});
    {
        LOCK(wallet->cs_wallet);
        list = wallet->ListCoins();
    }
    BOOST_CHECK_EQUAL(list.size(), 1U);
    BOOST_CHECK_EQUAL(boost::get<PKHash>(list.begin()->first).ToString(), coinbaseAddress);
    BOOST_CHECK_EQUAL(list.begin()->second.size(), 2U);

    // Lock both coins. Confirm number of available coins drops to 0.
    {
        LOCK(wallet->cs_wallet);
        std::vector<COutput> available;
        wallet->AvailableCoins(available);
        BOOST_CHECK_EQUAL(available.size(), 2U);
    }
    for (const auto& group : list) {
        for (const auto& coin : group.second) {
            LOCK(wallet->cs_wallet);
            wallet->LockCoin(COutPoint(coin.tx->GetHash(), coin.i));
        }
    }
    {
        LOCK(wallet->cs_wallet);
        std::vector<COutput> available;
        wallet->AvailableCoins(available);
        BOOST_CHECK_EQUAL(available.size(), 0U);
    }
    // Confirm ListCoins still returns same result as before, despite coins
    // being locked.
    {
        LOCK(wallet->cs_wallet);
        list = wallet->ListCoins();
    }
    BOOST_CHECK_EQUAL(list.size(), 1U);
    BOOST_CHECK_EQUAL(boost::get<PKHash>(list.begin()->first).ToString(), coinbaseAddress);
    BOOST_CHECK_EQUAL(list.begin()->second.size(), 2U);
}

BOOST_FIXTURE_TEST_CASE(wallet_disableprivkeys, TestChain240Setup)
{
    NodeContext node;
    auto chain = interfaces::MakeChain(node);
    std::shared_ptr<CWallet> wallet = std::make_shared<CWallet>(chain.get(), "", CreateDummyWalletDatabase());
    wallet->SetupLegacyScriptPubKeyMan();
    wallet->SetMinVersion(FEATURE_LATEST);
    wallet->SetWalletFlag(WALLET_FLAG_DISABLE_PRIVATE_KEYS);
    BOOST_CHECK(!wallet->TopUpKeyPool(1000));
    CTxDestination dest;
    std::string error;
    BOOST_CHECK(!wallet->GetNewDestination(OutputType::BECH32, "", dest, error));
}

// Explicit calculation which is used to test the wallet constant
// We get the same virtual size due to rounding(weight/4) for both use_max_sig values
static size_t CalculateNestedKeyhashInputSize(bool use_max_sig)
{
    // Generate ephemeral valid pubkey
    CKey key;
    key.MakeNewKey(true);
    CPubKey pubkey = key.GetPubKey();

    // Generate pubkey hash
    uint160 key_hash(Hash160(pubkey));

    // Create inner-script to enter into keystore. Key hash can't be 0...
    CScript inner_script = CScript() << OP_0 << std::vector<unsigned char>(key_hash.begin(), key_hash.end());

    // Create outer P2SH script for the output
    uint160 script_id(Hash160(inner_script));
    CScript script_pubkey = CScript() << OP_HASH160 << std::vector<unsigned char>(script_id.begin(), script_id.end()) << OP_EQUAL;

    // Add inner-script to key store and key to watchonly
    FillableSigningProvider keystore;
    keystore.AddCScript(inner_script);
    keystore.AddKeyPubKey(key, pubkey);

    // Fill in dummy signatures for fee calculation.
    SignatureData sig_data;

    if (!ProduceSignature(keystore, use_max_sig ? DUMMY_MAXIMUM_SIGNATURE_CREATOR : DUMMY_SIGNATURE_CREATOR, script_pubkey, sig_data)) {
        // We're hand-feeding it correct arguments; shouldn't happen
        assert(false);
    }

    CTxIn tx_in;
    UpdateInput(tx_in, sig_data);
    return (size_t)GetVirtualTransactionInputSize(tx_in);
}

BOOST_FIXTURE_TEST_CASE(dummy_input_size_test, TestChain240Setup)
{
    BOOST_CHECK_EQUAL(CalculateNestedKeyhashInputSize(false), DUMMY_NESTED_P2WPKH_INPUT_SIZE);
    BOOST_CHECK_EQUAL(CalculateNestedKeyhashInputSize(true), DUMMY_NESTED_P2WPKH_INPUT_SIZE);
}

bool malformed_descriptor(std::ios_base::failure e)
{
    std::string s(e.what());
    return s.find("Missing checksum") != std::string::npos;
}

BOOST_FIXTURE_TEST_CASE(wallet_descriptor_test, BasicTestingSetup)
{
    std::vector<unsigned char> malformed_record;
    CVectorWriter vw(0, 0, malformed_record, 0);
    vw << std::string("notadescriptor");
    vw << (uint64_t)0;
    vw << (int32_t)0;
    vw << (int32_t)0;
    vw << (int32_t)1;

    VectorReader vr(0, 0, malformed_record, 0);
    WalletDescriptor w_desc;
    BOOST_CHECK_EXCEPTION(vr >> w_desc, std::ios_base::failure, malformed_descriptor);
}

//! Test CWallet::Create() and its behavior handling potential race
//! conditions if it's called the same time an incoming transaction shows up in
//! the mempool or a new block.
//!
//! It isn't possible to verify there aren't race condition in every case, so
//! this test just checks two specific cases and ensures that timing of
//! notifications in these cases doesn't prevent the wallet from detecting
//! transactions.
//!
//! In the first case, block and mempool transactions are created before the
//! wallet is loaded, but notifications about these transactions are delayed
//! until after it is loaded. The notifications are superfluous in this case, so
//! the test verifies the transactions are detected before they arrive.
//!
//! In the second case, block and mempool transactions are created after the
//! wallet rescan and notifications are immediately synced, to verify the wallet
//! must already have a handler in place for them, and there's no gap after
//! rescanning where new transactions in new blocks could be lost.
BOOST_FIXTURE_TEST_CASE(CreateWallet, TestChain240Setup)
{
    // Create new wallet with known key and unload it.
    auto chain = interfaces::MakeChain(m_node);
    auto wallet = TestLoadWallet(*chain);
    CKey key;
    key.MakeNewKey(true);
    AddKey(*wallet, key);
    TestUnloadWallet(std::move(wallet));


    // Add log hook to detect AddToWallet events from rescans, blockConnected,
    // and transactionAddedToMempool notifications
    int addtx_count = 0;
    DebugLogHelper addtx_counter("[default wallet] AddToWallet", [&](const std::string* s) {
        if (s) ++addtx_count;
        return false;
    });


    bool rescan_completed = false;
    DebugLogHelper rescan_check("[default wallet] Rescan completed", [&](const std::string* s) {
        if (s) rescan_completed = true;
        return false;
    });


    // Block the queue to prevent the wallet receiving blockConnected and
    // transactionAddedToMempool notifications, and create block and mempool
    // transactions paying to the wallet
    std::promise<void> promise;
    CallFunctionInValidationInterfaceQueue([&promise] {
        promise.get_future().wait();
    });
    std::string error;
    m_coinbase_txns.push_back(CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);
    auto block_tx = TestSimpleSpend(*m_coinbase_txns[0], 0, coinbaseKey, GetScriptForRawPubKey(key.GetPubKey()));
    m_coinbase_txns.push_back(CreateAndProcessBlock({block_tx}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);
    auto mempool_tx = TestSimpleSpend(*m_coinbase_txns[1], 0, coinbaseKey, GetScriptForRawPubKey(key.GetPubKey()));
    BOOST_CHECK(chain->broadcastTransaction(MakeTransactionRef(mempool_tx), DEFAULT_TRANSACTION_MAXFEE, false, error));


    // Reload wallet and make sure new transactions are detected despite events
    // being blocked
    wallet = TestLoadWallet(*chain);
    BOOST_CHECK(rescan_completed);
    BOOST_CHECK_EQUAL(addtx_count, 2);
    {
        LOCK(wallet->cs_wallet);
        BOOST_CHECK_EQUAL(wallet->mapWallet.count(block_tx.GetHash()), 1U);
        BOOST_CHECK_EQUAL(wallet->mapWallet.count(mempool_tx.GetHash()), 1U);
    }


    // Unblock notification queue and make sure stale blockConnected and
    // transactionAddedToMempool events are processed
    promise.set_value();
    SyncWithValidationInterfaceQueue();
    BOOST_CHECK_EQUAL(addtx_count, 4);


    TestUnloadWallet(std::move(wallet));


    // Load wallet again, this time creating new block and mempool transactions
    // paying to the wallet as the wallet finishes loading and syncing the
    // queue so the events have to be handled immediately. Releasing the wallet
    // lock during the sync is a little artificial but is needed to avoid a
    // deadlock during the sync and simulates a new block notification happening
    // as soon as possible.
    addtx_count = 0;
    auto handler = HandleLoadWallet([&](std::unique_ptr<interfaces::Wallet> wallet) EXCLUSIVE_LOCKS_REQUIRED(wallet->wallet()->cs_wallet) {
            BOOST_CHECK(rescan_completed);
            m_coinbase_txns.push_back(CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);
            block_tx = TestSimpleSpend(*m_coinbase_txns[2], 0, coinbaseKey, GetScriptForRawPubKey(key.GetPubKey()));
            m_coinbase_txns.push_back(CreateAndProcessBlock({block_tx}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);
            mempool_tx = TestSimpleSpend(*m_coinbase_txns[3], 0, coinbaseKey, GetScriptForRawPubKey(key.GetPubKey()));
            BOOST_CHECK(chain->broadcastTransaction(MakeTransactionRef(mempool_tx), DEFAULT_TRANSACTION_MAXFEE, false, error));
            LEAVE_CRITICAL_SECTION(wallet->wallet()->cs_wallet);
            SyncWithValidationInterfaceQueue();
            ENTER_CRITICAL_SECTION(wallet->wallet()->cs_wallet);
        });
    wallet = TestLoadWallet(*chain);
    BOOST_CHECK_EQUAL(addtx_count, 4);
    {
        LOCK(wallet->cs_wallet);
        BOOST_CHECK_EQUAL(wallet->mapWallet.count(block_tx.GetHash()), 1U);
        BOOST_CHECK_EQUAL(wallet->mapWallet.count(mempool_tx.GetHash()), 1U);
    }


    TestUnloadWallet(std::move(wallet));
}

BOOST_FIXTURE_TEST_CASE(ZapSelectTx, TestChain240Setup)
{
    auto chain = interfaces::MakeChain(m_node);
    auto wallet = TestLoadWallet(*chain);
    CKey key;
    key.MakeNewKey(true);
    AddKey(*wallet, key);

    std::string error;
    m_coinbase_txns.push_back(CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);
    auto block_tx = TestSimpleSpend(*m_coinbase_txns[0], 0, coinbaseKey, GetScriptForRawPubKey(key.GetPubKey()));
    CreateAndProcessBlock({block_tx}, GetScriptForRawPubKey(coinbaseKey.GetPubKey()));

    SyncWithValidationInterfaceQueue();

    {
        auto block_hash = block_tx.GetHash();
        auto prev_hash = m_coinbase_txns[0]->GetHash();

        LOCK(wallet->cs_wallet);
        BOOST_CHECK(wallet->HasWalletSpend(prev_hash));
        BOOST_CHECK_EQUAL(wallet->mapWallet.count(block_hash), 1u);

        std::vector<uint256> vHashIn{ block_hash }, vHashOut;
        BOOST_CHECK_EQUAL(wallet->ZapSelectTx(vHashIn, vHashOut), DBErrors::LOAD_OK);

        BOOST_CHECK(!wallet->HasWalletSpend(prev_hash));
        BOOST_CHECK_EQUAL(wallet->mapWallet.count(block_hash), 0u);
    }

    TestUnloadWallet(std::move(wallet));
}

BOOST_FIXTURE_TEST_CASE(rescan, TestChain240Setup)
{
    LOCK(cs_main);

    // Cap last block file size, and mine new block in a new block file.
    CBlockIndex* oldTip = chainActive.Tip();
    GetBlockFileInfo(oldTip->GetBlockPos().nFile)->nSize = MAX_BLOCKFILE_SIZE;
    CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey()));
    CBlockIndex* newTip = chainActive.Tip();

    // Verify ScanForWalletTransactions picks up transactions in both the old
    // and new block files.
    {
        CWallet wallet;
        LOCK(wallet.cs_wallet);
        wallet.AddKeyPubKey(coinbaseKey, coinbaseKey.GetPubKey());
        BOOST_CHECK_EQUAL(oldTip, wallet.ScanForWalletTransactions(oldTip));
        BOOST_CHECK(wallet.GetImmatureBalance() < (240000000 * COIN));
    }

    // Prune the older block file.
    PruneOneBlockFile(oldTip->GetBlockPos().nFile);
    UnlinkPrunedFiles({oldTip->GetBlockPos().nFile});

    // Verify ScanForWalletTransactions only picks transactions in the new block
    // file.
    {
        CWallet wallet;
        LOCK(wallet.cs_wallet);
        wallet.AddKeyPubKey(coinbaseKey, coinbaseKey.GetPubKey());
        BOOST_CHECK_EQUAL(newTip, wallet.ScanForWalletTransactions(oldTip));
        BOOST_CHECK(wallet.GetImmatureBalance() < (120000000 * COIN));
    }

    // Verify importmulti RPC returns failure for a key whose creation time is
    // before the missing block, and success for a key whose creation time is
    // after.
    {
        CWallet wallet;
        CWallet *backup = ::pwalletMain;
        ::pwalletMain = &wallet;
        UniValue keys;
        keys.setArray();
        UniValue key;
        key.setObject();
        key.pushKV("scriptPubKey", HexStr(GetScriptForRawPubKey(coinbaseKey.GetPubKey())));
        key.pushKV("timestamp", 0);
        key.pushKV("internal", UniValue(true));
        keys.push_back(key);
        key.clear();
        key.setObject();
        CKey futureKey;
        futureKey.MakeNewKey(true);
        key.pushKV("scriptPubKey", HexStr(GetScriptForRawPubKey(futureKey.GetPubKey())));
        key.pushKV("timestamp", newTip->GetBlockTimeMax() + 7200);
        key.pushKV("internal", UniValue(true));
        keys.push_back(key);
        JSONRPCRequest request;
        request.params.setArray();
        request.params.push_back(keys);

        UniValue response = importmulti(request);
        BOOST_CHECK_EQUAL(response.write(), strprintf("[{\"success\":false,\"error\":{\"code\":-1,\"message\":\"Failed to rescan before time %d, transactions may be missing.\"}},{\"success\":true}]", newTip->GetBlockTimeMax()));
        ::pwalletMain = backup;
    }
}

// Verify importwallet RPC starts rescan at earliest block with timestamp
// greater or equal than key birthday. Previously there was a bug where
// importwallet RPC would start the scan at the latest block with timestamp less
// than or equal to key birthday.
BOOST_FIXTURE_TEST_CASE(importwallet_rescan, TestChain240Setup)
{
    CWallet *pwalletMainBackup = ::pwalletMain;
    LOCK(cs_main);

    // Create two blocks with same timestamp to verify that importwallet rescan
    // will pick up both blocks, not just the first.
    const int64_t BLOCK_TIME = chainActive.Tip()->GetBlockTimeMax() + 5;
    SetMockTime(BLOCK_TIME);
    coinbaseTxns.emplace_back(*CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);
    coinbaseTxns.emplace_back(*CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);

    // Set key birthday to block time increased by the timestamp window, so
    // rescan will start at the block time.
    const int64_t KEY_TIME = BLOCK_TIME + 7200;
    SetMockTime(KEY_TIME);
    coinbaseTxns.emplace_back(*CreateAndProcessBlock({}, GetScriptForRawPubKey(coinbaseKey.GetPubKey())).vtx[0]);

    // Import key into wallet and call dumpwallet to create backup file.
    {
        CWallet wallet;
        LOCK(wallet.cs_wallet);
        wallet.mapKeyMetadata[coinbaseKey.GetPubKey().GetID()].nCreateTime = KEY_TIME;
        wallet.AddKeyPubKey(coinbaseKey, coinbaseKey.GetPubKey());

        JSONRPCRequest request;
        request.params.setArray();
        request.params.push_back("wallet.backup");
        ::pwalletMain = &wallet;
        ::dumpwallet(request);
    }

    // Call importwallet RPC and verify all blocks with timestamps >= BLOCK_TIME
    // were scanned, and no prior blocks were scanned.
    {
        CWallet wallet;

        JSONRPCRequest request;
        request.params.setArray();
        request.params.push_back("wallet.backup");
        ::pwalletMain = &wallet;
        ::importwallet(request);

        BOOST_CHECK_EQUAL(wallet.mapWallet.size(), 3);
        BOOST_CHECK_EQUAL(coinbaseTxns.size(), 243);
        for (size_t i = 0; i < coinbaseTxns.size(); ++i) {
            bool found = wallet.GetWalletTx(coinbaseTxns[i].GetHash());
            bool expected = i >= 240;
            BOOST_CHECK_EQUAL(found, expected);
        }
    }

    SetMockTime(0);
    ::pwalletMain = pwalletMainBackup;
}

BOOST_AUTO_TEST_CASE(GetMinimumFee_test)
{
    uint64_t value = 1000 * COIN; // 1,000 DOGE

    CMutableTransaction tx;
    CTxMemPool pool(payTxFee);
    CTxOut txout1(value, (CScript)vector<unsigned char>(24, 0));
    tx.vout.push_back(txout1);

    int64_t nMinTxFee = COIN / 100;

    BOOST_CHECK_EQUAL(CWallet::GetMinimumFee(tx, 250, 0, pool), nMinTxFee * 0.25);
    BOOST_CHECK_EQUAL(CWallet::GetMinimumFee(tx, 1000, 0, pool), nMinTxFee * 1.0);
    BOOST_CHECK_EQUAL(CWallet::GetMinimumFee(tx, 1999, 0, pool), nMinTxFee * 1.999);
}

BOOST_AUTO_TEST_CASE(GetMinimumFee_dust_test)
{
    // Derived from main net TX 3d6ec3ae2aca3ae0a6c65074fd8ee888cd7ed262f2cbaa25d33861989324a14e
    CMutableTransaction tx;
    CTxMemPool pool(payTxFee);
    CTxOut txout1(139496846, (CScript)vector<unsigned char>(24, 0)); // Regular output
    CTxOut txout2(154996, (CScript)vector<unsigned char>(24, 0)); // Dust output
    tx.vout.push_back(txout1);
    tx.vout.push_back(txout2);

    CAmount nMinTxFee = COIN / 100;

    // Confirm dust penalty fees are added on
    // Because this is ran by the wallet, this takes the discardThreshold,
    // not the dust limit
    CAmount nDustPenalty = COIN;

    BOOST_CHECK_EQUAL(CWallet::GetMinimumFee(tx, 963, 0, pool), nDustPenalty + (nMinTxFee * 0.963));
    BOOST_CHECK_EQUAL(CWallet::GetMinimumFee(tx, 1000, 0, pool), nDustPenalty + (nMinTxFee * 1.000));
    BOOST_CHECK_EQUAL(CWallet::GetMinimumFee(tx, 1999, 0, pool), nDustPenalty + (nMinTxFee * 1.999));

    // change the discard threshold

    CWallet::discardThreshold = COIN / 1000;

    // Confirm dust penalty fees are not added

    BOOST_CHECK_EQUAL(CWallet::GetMinimumFee(tx, 963, 0, pool), nMinTxFee * 0.963);
    BOOST_CHECK_EQUAL(CWallet::GetMinimumFee(tx, 1000, 0, pool), nMinTxFee * 1.000);
    BOOST_CHECK_EQUAL(CWallet::GetMinimumFee(tx, 1999, 0, pool), nMinTxFee * 1.999);

    CWallet::discardThreshold = COIN;
}

BOOST_AUTO_TEST_SUITE_END()
