// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    const unsigned nHeight = pindexLast->nHeight + 1;
    const int64_t nInterval = params.DifficultyAdjustmentInterval(nHeight);

    // Only change once per difficulty adjustment interval
    if (nHeight % nInterval != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval(pindex->nHeight) != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    // Patch modified from Litecoin.
    int64_t blocksToGoBack = nInterval - 1;
    if (nHeight >= 43000 && nHeight != nInterval)
        blocksToGoBack = nInterval;

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - blocksToGoBack;
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    const unsigned nHeight = pindexLast->nHeight + 1;
    const int64_t nTargetTimespan = params.PowTargetTimespan(nHeight);

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (!params.RevisedIxcoin(nHeight)) {
      if (nActualTimespan < nTargetTimespan/4)
          nActualTimespan = nTargetTimespan/4;
      if (nActualTimespan > nTargetTimespan*4)
          nActualTimespan = nTargetTimespan*4;
    } else {
      //Altered version of SolidCoin.info retargetting code
      int64_t nTwoPercent = nTargetTimespan/50;
      //printf("  nActualTimespan = %"PRI64d"  before bounds\n", nActualTimespan);

      if (nActualTimespan < nTargetTimespan)  //is time taken for a block less than 10minutes?
      {
           //limit increase to a much lower amount than dictates to get past the pump-n-dump mining phase
          //due to retargets being done more often it also needs to be lowered significantly from the 4x increase
          if(nActualTimespan<(nTwoPercent*16)) //less than ~3.2 minute?
              nActualTimespan=(nTwoPercent*45); //pretend it was only 10% faster than desired
          else if(nActualTimespan<(nTwoPercent*32)) //less than ~6.4 minutes?
              nActualTimespan=(nTwoPercent*47); //pretend it was only 6% faster than desired
          else
              nActualTimespan=(nTwoPercent*49); //pretend it was only 2% faster than desired

          //int64 nTime=nTargetTimespan-nActualTimespan;
          //nActualTimespan = nTargetTimespan/2;
      }
      else if (nActualTimespan > nTargetTimespan*4)   nActualTimespan = nTargetTimespan*4;

    }

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
