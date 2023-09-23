#include "optimize.h"

double computeKValue(double kValue, std::vector<ErrorThread>& threads, const EvalParams& initial, const std::vector<Position>& positions)
{
	constexpr double delta = 1e-5;
	constexpr double deviationGoal = 1e-6;
	double deviation = 1;

	while (std::fabs(deviation) > deviationGoal)
	{
		double tuneUp = error(positions, initial, kValue + delta, threads);
		double tuneDown = error(positions, initial, kValue - delta, threads);
		deviation = (tuneUp - tuneDown) / (2 * delta);
		std::cout << "kValue: " << kValue << " up: " << tuneUp << " down: " << tuneDown << " deviation: " << deviation << std::endl;
		kValue -= deviation * 10;
	}

	return kValue;
}

EvalParams localSearch(std::vector<ErrorThread>& threads, double kValue, const EvalParams& initial, const std::vector<Position>& positions, std::ofstream& outFile)
{
	constexpr int DELTAS[] = {
		12, 5, 2, 1
	};
	constexpr int DELTA_CHANGE_MARGINS[] = {
		1, 2, 4, 0
	};

	int minImprovedIdx = 0;

	constexpr int MIN_IMPROVED_DIST[] = {
		2, 4, 7, 20, 10000
	};

	constexpr int MIN_IMPROVED_DIST_MARGINS[] = {
		25, 25, 25, 25, 0
	};

	double bestError = error(positions, initial, kValue, threads);
	EvalParams bestParams = initial;
	int it = 1;

	struct ParamInfo
	{
		int deltaIdx;
		int lastImprovedIt;
	} paramsInfo[NUM_PARAMS] = {};

	int numImproved;
	int numLargeDelta;
	do
	{
		numImproved = 0;
		numLargeDelta = 0;

		int minImprovedDist = MIN_IMPROVED_DIST[minImprovedIdx];
		for (uint32_t i = 0; i < NUM_PARAMS; i++)
		{
			int improvedCount = 0;
			auto& paramInfo = paramsInfo[i];
			if (paramInfo.deltaIdx != std::size(DELTAS) - 1)
				numLargeDelta++;
			if (paramInfo.lastImprovedIt < it - minImprovedDist)
			{
				std::cout << "Skipping, not improved in a while " << std::endl;
				continue;
			}
			for (;;)
			{
				for (int j = 0; j < 2; j++)
				{
					int delta = DELTAS[paramInfo.deltaIdx] * (j == 0 ? 1 : -1);
					EvalParams newParams = bestParams;
					newParams.params[i] += delta;
					double newError = error(positions, newParams, kValue, threads);
					std::cout << "Param: " << i << " Delta: " << delta << " New: " << newError << " best: " << bestError << '\n';
					if (newError < bestError)
					{
						bestError = newError;
						bestParams = newParams;
						improvedCount++;
						goto improved;
					}
					newParams.params[i] -= delta;
				}
				break;
			improved:
				std::cout << "Improved" << std::endl;
			}
			if (improvedCount > 0)
			{
				paramInfo.lastImprovedIt = it;
				numImproved++;
			}
			if (improvedCount < DELTA_CHANGE_MARGINS[paramInfo.deltaIdx])
			{
				std::cout << "Decreasing delta" << std::endl;
				paramInfo.deltaIdx++;
			}
		}

		if (numImproved < MIN_IMPROVED_DIST_MARGINS[minImprovedIdx])
			minImprovedIdx++;

		std::cout << "Iteration: " << it << std::endl;
		outFile << "Iteration: " << it++ << std::endl;
		std::cout << "Error: " << bestError << std::endl;
		outFile << "Error: " << bestError << std::endl;
		std::cout << "Num improved: " << numImproved << std::endl;
		outFile << "Num improved: " << numImproved << std::endl;
		std::cout << "Num large delta: " << numLargeDelta << std::endl;
		outFile << "Num large delta: " << numLargeDelta << std::endl;
		printParams(bestParams, std::cout);
		printParams(bestParams, outFile);
	}
	while (numImproved > 0 || numLargeDelta > 0);

	return bestParams;
}

/*void normalize(const EvalParams& params)
{
	EvalParams paramsCopy = params;
	EvalData& data = paramsCopy.data;

	for (int i = 1; i < 6; i++)
	{
		int mgPsqtSum = 0;
		int egPsqtSum = 0;
		for (int j = 0; j < 32; j++)
		{
			mgPsqtSum += data.psqtMG[i][j];
			egPsqtSum += data.psqtEG[i][j];
		}
		std::cout << "Piece: " << i << " OLD: " << std::endl;
		std::cout << data.materialMG[i] << ' ' << mgPsqtSum << std::endl;
		std::cout << data.materialEG[i] << ' ' << egPsqtSum << std::endl;

		while (mgPsqtSum > 16)
		{
			mgPsqtSum -= 32;
			for (int j = 0; j < 32; j++)
			{
				data.psqtMG[i][j] -= 1;
			}
			data.materialMG[i] += 1;
		}
		while (mgPsqtSum < -16)
		{
			mgPsqtSum += 32;
			for (int j = 0; j < 32; j++)
			{
				data.psqtMG[i][j] += 1;
			}
			data.materialMG[i] -= 1;
		}

		while (egPsqtSum > 31)
		{
			egPsqtSum -= 32;
			for (int j = 0; j < 32; j++)
			{
				data.psqtEG[i][j] -= 1;
			}
			data.materialEG[i] += 1;
		}
		while (egPsqtSum < 0)
		{
			egPsqtSum += 32;
			for (int j = 0; j < 32; j++)
			{
				data.psqtEG[i][j] += 1;
			}
			data.materialEG[i] -= 1;
		}

		std::cout << "Piece: " << i << " NEW: " << std::endl;
		std::cout << data.materialMG[i] << ' ' << mgPsqtSum << std::endl;
		std::cout << data.materialEG[i] << ' ' << egPsqtSum << std::endl;
		std::cout << std::endl;
	}

	printParams(paramsCopy, std::cout);
}

void normalizeBytes(EvalParams params)
{
	EvalData& data = params.data;

	for (int i = 6; i < NUM_PARAMS; i++)
	{
		params.params[i] *= 2;
	}
	printParams(params, std::cout);
	return;

	int min = INT_MAX;
	int max = INT_MIN;
	for (int* p = reinterpret_cast<int*>(data.psqtMG); p != params.params + NUM_PARAMS; p++)
	{
		if (*p > max)
			max = *p;
		if (*p < min)
			min = *p;
	}

	int range = max - min;
	std::cout << "Mult Factor: " << 255 << " / " << range << std::endl;

	int diff = 0;
	// too lazy to figure out the mathematical approach without loops
	while (-min > max)
	{
		diff++;
		min++;
		max++;
	}
	while (-min < max)
	{
		diff--;
		min--;
		max--;
	}

	std::cout << "New min: " << min << " New max: " << max << " diff: " << diff << std::endl;

	for (int i = 1; i < 6; i++)
	{
		data.materialMG[i] -= diff;
		data.materialMG[i] = static_cast<int>(std::round(data.materialMG[i] * 255.0 / range));
		data.materialEG[i] -= diff;
		data.materialEG[i] = static_cast<int>(std::round(data.materialEG[i] * 255.0 / range));
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			data.psqtMG[i][j] += diff;
			data.psqtMG[i][j] = static_cast<int>(std::round(data.psqtMG[i][j] * 255.0 / range));
			data.psqtEG[i][j] += diff;
			data.psqtEG[i][j] = static_cast<int>(std::round(data.psqtEG[i][j] * 255.0 / range));
		}
	}

	printParams(params, std::cout);
}*/
