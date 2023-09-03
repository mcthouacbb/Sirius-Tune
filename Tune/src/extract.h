#include "eval_params.h"
#include <uint128_t/uint128_t.h>

#include <array>

void extractMaterial(const EvalParams& params);
void normBytes(EvalParams& params, std::array<std::array<int, 6>, 2>& material);
std::vector<uint128_t> compressParams(const EvalParams& params, const std::array<std::array<int, 6>, 2>& material);