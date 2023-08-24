#include "optimize.h"

#include <unordered_map>

std::unordered_map<int, int> deltaToIdx = {
	{-5, 0},
	{-1, 1},
	{1, 2},
	{5, 3}
};

int deltaOrders[5][4] = {
	{-5, -1, 5, 1},
	{-1, -5, 5, 1},
	{1, 5, -5, -1},
	{5, 1, -5, -1},
	{5, -5, 1, -1}
};

double computeKValue(std::vector<ErrorThread>& threads, const EvalParams& initial, const std::vector<Position>& positions)
{
	constexpr double delta = 1e-5;
	constexpr double deviationGoal = 1e-6;
	double deviation = 1;

	double kValue = 3.68058;

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
	double bestError = error(positions, initial, kValue, threads);
	EvalParams bestParams = initial;
	bool improved = true;
	int it = 1;
	while (improved)
	{
		improved = false;
		for (uint32_t i = 0; i < NUM_PARAMS; i++)
		{
			bool improvedParam = false;
			int prevIdx = 4;
			for (;;)
			{
				for (int j = 0; j < 4; j++)
				{
					int delta = deltaOrders[prevIdx][j];
					EvalParams newParams = bestParams;
					newParams.params[i] += delta;
					double newError = error(positions, newParams, kValue, threads);
					std::cout << "Param: " << i << " Delta: " << delta << " New: " << newError << " best: " << bestError << '\n';
					if (newError < bestError)
					{
						bestError = newError;
						bestParams = newParams;
						improved = true;
						prevIdx = deltaToIdx.at(delta);
						goto improved;
					}
					newParams.params[i] -= delta;
				}
				break;
			improved:
				std::cout << "Improved" << std::endl;
			}
		}
		std::cout << "Iteration: " << it << std::endl;
		outFile << "Iteration: " << it++ << std::endl;
		std::cout << "Error: " << bestError << std::endl;
		outFile << "Error: " << bestError << std::endl;
		printParams(bestParams, std::cout);
		printParams(bestParams, outFile);
	}
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