#pragma once

#include "error.h"
#include <fstream>


double computeKValue(std::vector<ErrorThread>& threads, const EvalParams& initial, const std::vector<Position>& positions);

EvalParams localSearch(std::vector<ErrorThread>& threads, double kValue, const EvalParams& initial, const std::vector<Position>& positions, std::ofstream& outFile);

//void normalize(const EvalParams& params);
//void normalizeBytes(EvalParams);