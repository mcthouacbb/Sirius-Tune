#pragma once

#include <iostream>

#include "pos.h"

struct EvalData
{
    int psqtMG[6 * 64];
    int psqtEG[6 * 64];
    int bishopPairMG;
    int bishopPairEG;
};

constexpr size_t NUM_PARAMS = sizeof(EvalData) / sizeof(int);

union EvalParams
{
    EvalData data;
    int params[NUM_PARAMS];
};

void printParams(const EvalParams& params, std::ostream& os);
int evaluate(const Position& position, const EvalParams& params);