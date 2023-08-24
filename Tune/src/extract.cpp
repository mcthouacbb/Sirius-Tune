#include "extract.h"

/*void extractMaterial(const EvalParams& params)
{
    int material[2][6] = {};
    for (int pce = 1; pce < 6; pce++)
    {
        int mgAvg = 0;
        int egAvg = 0;
        for (int sq = 0; sq < 64; sq++)
        {
            mgAvg += params.data.psqtMG[pce * 64 + sq];
            egAvg += params.data.psqtEG[pce * 64 + sq];
        }

        material[0][pce] = mgAvg / (pce == 5 ? 48 : 64);
        material[1][pce] = egAvg / (pce == 5 ? 48 : 64);
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
}*/