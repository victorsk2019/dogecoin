// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/pureheader.h"

#include "chainparams.h"
#include "crypto/scrypt.h"
#include "hash.h"
#include "utilstrencodings.h"
=======
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Copyright (c) 2021 The Dogecoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/pureheader.h>

#include <crypto/scrypt.h>
#include <hash.h>
#include <util/strencodings.h>
>>>>>>> 1.21-dev

void CPureBlockHeader::SetBaseVersion(int32_t nBaseVersion, int32_t nChainId)
{
    assert(nBaseVersion >= 1 && nBaseVersion < VERSION_AUXPOW);
    assert(!IsAuxpow());
    nVersion = nBaseVersion | (nChainId * VERSION_CHAIN_START);
}

uint256 CPureBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CPureBlockHeader::GetPoWHash() const
{
    uint256 thash;
<<<<<<< HEAD
    scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(thash));
=======
    scrypt_1024_1_1_256((char*)&nVersion, (char*)&thash);
>>>>>>> 1.21-dev
    return thash;
}
