#pragma once

#include "error.h"
#include <fstream>


double computeKValue(double kValue, std::vector<ErrorThread>& threads, const EvalParams& initial, const std::vector<Position>& positions);

EvalParams localSearch(double kValue, std::vector<ErrorThread>& threads, const EvalParams& initial, const std::vector<Position>& positions, std::ofstream& outFile);

//void normalize(const EvalParams& params);
//void normalizeBytes(EvalParams);