// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (  0, uint256("0x8c0d74f0126f18dfc3e8651d6dd9da7c54beee28f84dbc1d3b8349fae64a2f8b"))
	( 39, uint256("0x4788cb74e7f487a8a2c801e7f743c4cefb309368acf82c8bd3e187fb11ff9a8d"))
	(156, uint256("0xdb0852c46c2badffeb2f64be69f807df4ce357efad2bdfc15b65c07a5ee58ac4"))
	(297, uint256("0x8f39de1d4a159069b6a3145a7820b2c42e7f896dac5d71e193f9963ea52f775a"))
	(5584, uint256("0x12ae50c13bc15e0901e42ef51dd48823ab8fefa5ee7da3621e7f42559d7af549"))
	(16893, uint256("0xaeba157d21a48ad2eebdd8fda128b67aacfdc32f1986c048480169850b70a3bc"))
	(18269, uint256("0x5c283c5739048c561521bc0c214205b528ee36cbd5fba7ee1e29a4eb966a4b9d"))
	(70065, uint256("0x46839a1fe3f72b774d776b55e11f55c2ad97e3d08efd4f54e5fcf2e0df091920"))
       (110065, uint256("0x4cdc0ed4f589fd3e19020d9cc62f800e450e65bf75c590ac2137554b3e51fa2b"))
       (140065, uint256("0x5ce28f37b259e1e90fba88c3d9b21ac84c65156dd91b789514d621225504f187"))
       (145065, uint256("0xd4913363f5362e3047d89fc9056267f65a8c060ab339e2b06f589f740775c425"))
       (150065, uint256("0xf39ad2902d4e7e5f412eec0dc0fcc9d955864a267bfc1fcd1f7708dc2da8afce"))
       (155065, uint256("0xff9bc46942d0ece1a57b49458a611cbc6e993386aef24a8e74f6c5fc2f0a247f"))
       (155791, uint256("0xe28f06d74bc8fc2b8e62ffccd82fd427d1e96afc75f589cc804ccf025da10af8"))
       (155792, uint256("0xe88b27c26813401ce7cd20f5a500ced38d1ef2e1acf762a603409cba73a3188a"))
       (155793, uint256("0x37f14d5ce04ba53d1b87fc1fabf2ad294062bf40d87772ac89a7de14d1173b90"))
       (155794, uint256("0xc3dab09ec1165fdb867c90e478dc50cc41174ddebbb0b835cbeb284a3cfc9b29"))
       (155795, uint256("0xf7b54d608760623a031082e971bda09e3e956cbbd2182bb97e1597046df3c674"))
       (155800, uint256("0xce1ecc4a3f7815fce68dc0484d3b56a63d483b07b73e79715506210660ebb13e"))
       (156065, uint256("0x716038bb3abf6c41c3a48b9efbb8446df431ba9aa4caf0816daa17c185420616"))
       (157065, uint256("0x9c496e465f317d5ae30cb05c82d0cbdccbccb28e0a91e22bae61df891cba781e"))
       (158065, uint256("0xa8f449593865d7de73380355ff02de4e32d7afbd9be157e06048903ea4175d90"))
       (159065, uint256("0xf510094e798ab81e5988b27f4671f1bd56b5a8f14ac03f10937200055c4f50ad"))
       (160065, uint256("0x79e11550860d9836b42e3c3d171b5ee2d6a2e230cef443b48a5cc9e7b5788ef0"))
       (161065, uint256("0x3c60f96a65aaa206b7efad0a528ae1fb9608b9d5ffb9019b4c803222a220ba65"))
       (162065, uint256("0xef09e9d755c1003831d65e2049e6ee62227eed4aade1a351b3b2e30e7c96973b"))
       (163065, uint256("0x35f8d5d03828ecd123d8b8c7eac6f83c58ec31506cc318cbe82202af4fcd6ec0"))
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1482794642, // * UNIX timestamp of last checkpoint block
        50000,    // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        500.0     // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        (   0, uint256("0x"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        //1365458829,
        //547,
        //576
    };

    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!GetBoolArg("-checkpoints", true))
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!GetBoolArg("-checkpoints", true))
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!GetBoolArg("-checkpoints", true))
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
