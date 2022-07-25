// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_PRIMITIVES_BLOCK_H
#define BITCOIN_PRIMITIVES_BLOCK_H

<<<<<<< HEAD
#include "auxpow.h"
#include "primitives/transaction.h"
#include "primitives/pureheader.h"
#include "serialize.h"
#include "uint256.h"
=======
#include <auxpow.h>
#include <primitives/transaction.h>
#include <primitives/pureheader.h>
#include <serialize.h>
#include <uint256.h>

#include <memory>
>>>>>>> 1.21-dev

#include <boost/shared_ptr.hpp>

/** Nodes collect new transactions into a block, hash them into a hash tree,
 * and scan through nonce values to make the block's hash satisfy proof-of-work
 * requirements.  When they solve the proof-of-work, they broadcast the block
 * to everyone and the block is added to the block chain.  The first transaction
 * in the block is a special one that creates a new coin owned by the creator
 * of the block.
 */
class CBlockHeader : public CPureBlockHeader
{
public:
<<<<<<< HEAD
    // auxpow (if this is a merge-minded block)
    boost::shared_ptr<CAuxPow> auxpow;
=======

    // auxpow (if this is a merge-minded block)
    std::shared_ptr<CAuxPow> auxpow;
>>>>>>> 1.21-dev

    CBlockHeader()
    {
        SetNull();
    }

    SERIALIZE_METHODS(CBlockHeader, obj)
    {
        READWRITEAS(CPureBlockHeader, obj);

<<<<<<< HEAD
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(*(CPureBlockHeader*)this);

        if (this->IsAuxpow())
        {
            if (ser_action.ForRead())
                auxpow.reset(new CAuxPow());
            assert(auxpow);
            READWRITE(*auxpow);
        } else if (ser_action.ForRead())
            auxpow.reset();
=======
        if (obj.IsAuxpow())
        {
            SER_READ(obj, obj.auxpow = std::make_shared<CAuxPow>());
            assert(obj.auxpow != nullptr);
            READWRITE(*obj.auxpow);
        } else
        {
            SER_READ(obj, obj.auxpow.reset());
        }
>>>>>>> 1.21-dev
    }

    void SetNull()
    {
        CPureBlockHeader::SetNull();
        auxpow.reset();
    }

    /**
     * Set the block's auxpow (or unset it).  This takes care of updating
     * the version accordingly.
<<<<<<< HEAD
     * @param apow Pointer to the auxpow to use or NULL.
     */
    void SetAuxpow(CAuxPow* apow);
=======
     */
    void SetAuxpow (std::unique_ptr<CAuxPow> apow);
>>>>>>> 1.21-dev
};


class CBlock : public CBlockHeader
{
public:
    // network and disk
    std::vector<CTransactionRef> vtx;

    // memory only
    mutable bool fChecked;

    CBlock()
    {
        SetNull();
    }

    CBlock(const CBlockHeader &header)
    {
        SetNull();
        *(static_cast<CBlockHeader*>(this)) = header;
    }

    SERIALIZE_METHODS(CBlock, obj)
    {
        READWRITEAS(CBlockHeader, obj);
        READWRITE(obj.vtx);
    }

    void SetNull()
    {
        CBlockHeader::SetNull();
        vtx.clear();
        fChecked = false;
    }

    CBlockHeader GetBlockHeader() const
    {
        CBlockHeader block;
        block.nVersion       = nVersion;
        block.hashPrevBlock  = hashPrevBlock;
        block.hashMerkleRoot = hashMerkleRoot;
        block.nTime          = nTime;
        block.nBits          = nBits;
        block.nNonce         = nNonce;
        block.auxpow         = auxpow;
        return block;
    }

    // Build the in-memory merkle tree for this block and return the merkle root.
    // If non-NULL, *mutated is set to whether mutation was detected in the merkle
    // tree (a duplication of transactions in the block leading to an identical
    // merkle root).
    uint256 BuildMerkleTree(bool* mutated = NULL) const;

    std::vector<uint256> GetMerkleBranch(int nIndex) const;
    static uint256 CheckMerkleBranch(uint256 hash, const std::vector<uint256>& vMerkleBranch, int nIndex);
    std::string ToString() const;
};

/** Describes a place in the block chain to another node such that if the
 * other node doesn't have the same branch, it can find a recent common trunk.
 * The further back it is, the further before the fork it may be.
 */
struct CBlockLocator
{
    std::vector<uint256> vHave;

    CBlockLocator() {}

    explicit CBlockLocator(const std::vector<uint256>& vHaveIn) : vHave(vHaveIn) {}

    SERIALIZE_METHODS(CBlockLocator, obj)
    {
        int nVersion = s.GetVersion();
        if (!(s.GetType() & SER_GETHASH))
            READWRITE(nVersion);
        READWRITE(obj.vHave);
    }

    void SetNull()
    {
        vHave.clear();
    }

    bool IsNull() const
    {
        return vHave.empty();
    }
};

#endif // BITCOIN_PRIMITIVES_BLOCK_H
