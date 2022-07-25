// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <crypto/common.h>
#include <crypto/scrypt.h>

<<<<<<< HEAD
void CBlockHeader::SetAuxpow (CAuxPow* apow)
{
    if (apow)
    {
        auxpow.reset(apow);
=======
void CBlockHeader::SetAuxpow (std::unique_ptr<CAuxPow> apow)
{
    if (apow != nullptr)
    {
        auxpow.reset(apow.release());
>>>>>>> 1.21-dev
        SetAuxpowFlag(true);
    } else
    {
        auxpow.reset();
        SetAuxpowFlag(false);
    }
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
