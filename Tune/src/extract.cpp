#include "extract.h"

void extractMaterial(const EvalParams& params)
{
    int material[2][6] = {};
    for (int pce = 1; pce < 6; pce++)
    {
        int mgAvg = 0;
        int egAvg = 0;
	int startSquare = pce == 5 ? 24 : 0;
	int endSquare = pce == 5 ? 56 : 64;
        for (int sq = startSquare; sq < endSquare; sq++)
        {
            mgAvg += params.data.psqtMG[pce * 64 + sq];
            egAvg += params.data.psqtEG[pce * 64 + sq];
        }

        material[0][pce] = mgAvg / (endSquare - startSquare);
        material[1][pce] = egAvg / (endSquare - startSquare);
    }

    for (int i = 0; i < 2; i++)
    {
        std::cout << material[i][0];
        for (int j = 1; j < 6; j++)
        {
            std::cout << ", " << material[i][j];
        }
        std::cout << std::endl;
    }

    EvalData data = params.data;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            data.psqtMG[i * 64 + j] -= material[0][i];
            data.psqtEG[i * 64 + j] -= material[1][i];
        }
    }

    printParams(EvalParams(data), std::cout);
}

void normBytes(EvalParams params)
{
	EvalData& data = params.data;

	int material[2][6] = {};

	for (int i = 0; i < 6; i++)
	{
		int mgPsqtSum = 0;
		int egPsqtSum = 0;
		for (int j = 0; j < 64; j++)
		{
			mgPsqtSum += data.psqtMG[i * 64 + j];
			egPsqtSum += data.psqtEG[i * 64 + j];
		}

		while (mgPsqtSum > 32)
		{
			mgPsqtSum -= 64;
			for (int j = 0; j < 64; j++)
				data.psqtMG[i * 64 + j] -= 1;
			if (i != 0)
				material[0][i] += 1;
		}
		while (mgPsqtSum <= -32)
		{
			mgPsqtSum += 64;
			for (int j = 0; j < 64; j++)
				data.psqtMG[i * 64 + j] += 1;
			if (i != 0)
				material[0][i] -= 1;
		}

		while (egPsqtSum > 32)
		{
			egPsqtSum -= 64;
			for (int j = 0; j < 64; j++)
				data.psqtEG[i * 64 + j] -= 1;
			if (i != 0)
				material[1][i] += 1;
		}
		while (egPsqtSum <= -32)
		{
			egPsqtSum += 64;
			for (int j = 0; j < 64; j++)
				data.psqtEG[i * 64 + j] += 1;
			if (i != 0)
				material[1][i] -= 1;
		}
	}


    for (int i = 0; i < 2; i++)
    {
        std::cout << material[i][0];
        for (int j = 1; j < 6; j++)
        {
            std::cout << ", " << material[i][j];
        }
        std::cout << std::endl;
    }
	printParams(params, std::cout);
	

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
		material[0][i] -= diff;
		material[0][i] = static_cast<int>(std::round(material[0][i] * 255.0 / range));
		material[1][i] -= diff;
		material[1][i] = static_cast<int>(std::round(material[1][i] * 255.0 / range));
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			data.psqtMG[i * 64 + j] += diff;
			data.psqtMG[i * 64 + j] = static_cast<int>(std::round(data.psqtMG[i * 64 + j] * 255.0 / range));
			data.psqtEG[i * 64 + j] += diff;
			data.psqtEG[i * 64 + j] = static_cast<int>(std::round(data.psqtEG[i * 64 + j] * 255.0 / range));
		}
	}

    for (int i = 0; i < 2; i++)
    {
        std::cout << material[i][0];
        for (int j = 1; j < 6; j++)
        {
            std::cout << ", " << material[i][j];
        }
        std::cout << std::endl;
    }
	printParams(params, std::cout);
}