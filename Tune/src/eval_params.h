#pragma once

#include <iostream>

#include "board.h"

struct EvalData
{
    int phaseWeights[6];
    int materialMG[6];
    int materialEG[6];
    int psqtMG[6][64];
    int psqtEG[6][64];
};

struct EvalCache
{
    int totalPhase;
};

constexpr size_t NUM_PARAMS = sizeof(EvalData) / sizeof(int);

union EvalParams
{
    EvalData data;
    int params[NUM_PARAMS];
};

void printParams(const EvalParams& params, std::ostream& os);
bool isDegenerate(const EvalParams& params);
EvalCache genCache(const EvalParams& params);
int evaluate(const Board& board, const EvalParams& params, const EvalCache& cache);