#include "extract.h"
#include <cassert>

void extractMaterial(const EvalParams& params)
{
    std::array<std::array<int, 6>, 2> material = {};
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

std::vector<uint128_t> compressPsts(const EvalParams& params, const std::array<std::array<int, 6>, 2>& material)
{
    EvalParams flipped = {};
    for (int i = 0; i < 6; i++)
    {
        for (int sq = 0; sq < 64; sq++)
        {
            flipped.data.psqtMG[i * 64 + sq] = params.data.psqtMG[(5 - i) * 64 + sq];
            flipped.data.psqtEG[i * 64 + sq] = params.data.psqtEG[(5 - i) * 64 + sq];
        }
    }
    printParams(flipped, std::cout);
    std::vector<uint128_t> compressed = {};
    for (int i = 0; i < 384; i += 12)
    {
        uint128_t x = 0;
        for (int j = i + 11; j >= i; j--)
        {
            assert(flipped.data.psqtMG[j] <= 255 && flipped.data.psqtMG[j] >= 0 && "Parameter is outside byte range");
            x <<= 8;
            x |= (uint8_t)flipped.data.psqtMG[j];
        }
        compressed.push_back(x);
    }

    for (int i = 0; i < 384; i += 12)
    {
        uint128_t x = 0;
        for (int j = i + 11; j >= i; j--)
        {
            assert(flipped.data.psqtEG[j] <= 255 && flipped.data.psqtEG[j] >= 0 && "Parameter is outside byte range");
            x <<= 8;
            x |= (uint8_t)flipped.data.psqtEG[j];
        }
        compressed.push_back(x);
    }
    return compressed;
}

void normBytes(EvalParams& params, std::array<std::array<int, 6>, 2>& material)
{
    EvalData& data = params.data;

    material = {};

    int maxRange = 0;
    for (int i = 0; i < 6; i++)
    {
        int mgPsqtMin = 100000;
        int mgPsqtMax = -100000;
        int egPsqtMin = 100000;
        int egPsqtMax = -100000;
        int startSquare = i == 5 ? 8 : 0;
        int endSquare = i == 5 ? 56 : 64;
        for (int j = startSquare; j < endSquare; j++)
        {
            mgPsqtMin = std::min(mgPsqtMin, data.psqtMG[i * 64 + j]);
            mgPsqtMax = std::max(mgPsqtMax, data.psqtMG[i * 64 + j]);
            egPsqtMin = std::min(egPsqtMin, data.psqtEG[i * 64 + j]);
            egPsqtMax = std::max(egPsqtMax, data.psqtEG[i * 64 + j]);
        }

        maxRange = std::max(std::max(maxRange, mgPsqtMax - mgPsqtMin), egPsqtMax - egPsqtMin);

        while (mgPsqtMin > 0)
        {
            mgPsqtMin -= 1;
            for (int j = startSquare; j < endSquare; j++)
                data.psqtMG[i * 64 + j] -= 1;
            if (i != 0)
                material[0][i] += 1;
        }
        while (mgPsqtMin < 0)
        {
            mgPsqtMin += 1;
            for (int j = startSquare; j < endSquare; j++)
                data.psqtMG[i * 64 + j] += 1;
            if (i != 0)
                material[0][i] -= 1;
        }

        while (egPsqtMin > 0)
        {
            egPsqtMin -= 1;
            for (int j = startSquare; j < endSquare; j++)
                data.psqtEG[i * 64 + j] -= 1;
            if (i != 0)
                material[1][i] += 1;
        }
        while (egPsqtMin < 0)
        {
            egPsqtMin += 1;
            for (int j = startSquare; j < endSquare; j++)
                data.psqtEG[i * 64 + j] += 1;
            if (i != 0)
                material[1][i] -= 1;
        }
    }

    if (maxRange > 255)
    {
        int mul = 255;
        int div = maxRange;
        for (int i = 0; i < 6; i++)
        {
            material[0][i] = material[0][i] * mul / div;
            material[1][i] = material[1][i] * mul / div;
        }
        for (int i = 0; i < NUM_PARAMS; i++)
        {
            params.params[i] = params.params[i] * mul / div;
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
