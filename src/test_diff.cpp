// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "uint256.h"

// to obtain PRId64 on some old systems
#define __STDC_FORMAT_MACROS 1

#include <stdint.h>
#include <inttypes.h>


using namespace std;

#define DEBUG_DIFFICULTY


double GetDifficultyFromTarget(uint32_t nBits)
{
    uint64_t numerator = 0x0000FFFF;
    uint64_t denominator = nBits & 0x00FFFFFF;

#ifdef DEBUG_DIFFICULTY
    printf("numerator: %016" PRIx64 ", denominator: %016" PRIx64"\n", numerator, denominator);
#endif

    int nShift = 29 - ((nBits >> 24) & 0xff);
#ifdef DEBUG_DIFFICULTY
    printf("nShift: %d\n", nShift);
#endif
    if (nShift < 0) {
        denominator <<= -8 * nShift;
    } else if (nShift > 0) {
        numerator <<= 8 * nShift;
    }

#ifdef DEBUG_DIFFICULTY
    printf("numerator: %016" PRIx64 ", denominator: %016" PRIx64 "\n", numerator, denominator);
#endif

    double dDiff = (double)numerator / (double)denominator;
#ifdef DEBUG_DIFFICULTY
    printf("diff: %0.8f\n", dDiff);
#endif
    return dDiff;
}

uint32_t GetTargetFromDifficulty(double difficulty)
{
    if (difficulty <= 0.0000000001) {
	return 0;
    }

    double numerator = (double)(0x0000FFFF);
    double denominator = numerator / difficulty;
    int nShift = 29;

#ifdef DEBUG_DIFFICULTY
    printf("numerator: %0.9f, denominator: %0.9f, nShift: %d\n",
           numerator, denominator, nShift);
#endif

    double limit = (double)(0x00007FFF);
    while (denominator <= limit) {
	denominator *= 256.0;
	nShift -= 1;
    }

#ifdef DEBUG_DIFFICULTY
    printf("limit: %0.9f, denominator: %0.9f, nShift: %d\n",
           limit, denominator, nShift);
#endif

    limit = (double)(0x007FFFFF);
    while (denominator > limit) {
	denominator /= 256.0;
	nShift += 1;
    }

#ifdef DEBUG_DIFFICULTY
    printf("limit: %0.9f, denominator: %0.9f, nShift: %d\n",
           limit, denominator, nShift);
#endif

    if (nShift < 0 || nShift > 255) {
	return 0;
    }

    uint32_t target = ((nShift & 0xFF) << 24);
    target |= (uint32_t)(denominator) & 0x007FFFFF;

#ifdef DEBUG_DIFFICULTY
    printf("target: %08X\n", target);
#endif

    return target;
}


int main(void)
{
    uint32_t nBitsIn, nBitsOut;
    double difficultyIn, difficultyOut;

    difficultyIn = 2.88488628;
    nBitsIn = GetTargetFromDifficulty(difficultyIn);
    difficultyOut = GetDifficultyFromTarget(nBitsIn);
    nBitsOut = GetTargetFromDifficulty(difficultyOut);

    printf("diff in: %0.8f, nBits in: 0x%08X, diff out: %0.8f, nBits out: 0x%08X\n", difficultyIn, nBitsIn, difficultyOut, nBitsOut);

    difficultyIn = (float)2.88488628;
    nBitsIn = GetTargetFromDifficulty(difficultyIn);
    difficultyOut = GetDifficultyFromTarget(nBitsIn);
    nBitsOut = GetTargetFromDifficulty(difficultyOut);

    printf("diff in: %0.8f, nBits in: 0x%08X, diff out: %0.8f, nBits out: 0x%08X\n", difficultyIn, nBitsIn, difficultyOut, nBitsOut);
}
