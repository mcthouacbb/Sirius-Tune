#pragma once

#include <iostream>

#include "pos.h"

struct EvalData
{
    int materialMG[6];
    int materialEG[6];
    int fileMG[6 * 8];
    int fileEG[6 * 8];
    int rankMG[6 * 8];
    int rankEG[6 * 8];
};

constexpr size_t NUM_PARAMS = sizeof(EvalData) / sizeof(int);

union EvalParams
{
    EvalData data;
    int params[NUM_PARAMS];
};

void printParams(const EvalParams& params, std::ostream& os);
int evaluate(const Position& position, const EvalParams& params);