#include "eval_params.h"

inline int numDigits(int num)
{
    int abse = std::abs(num);
    int digits = 1;
    while (abse / 10 != 0)
    {
        abse /= 10;
        digits++;
    }
    return digits + (num < 0);
}

void printParams(const EvalParams& params, std::ostream& os)
{
    const EvalData& data = params.data;
    os << "{\n";

    os << "\t{\n";
    for (int i = 0; i < 6; i++)
    {
        os << "\t\t{\n";
        for (int y = 0; y < 8; y++)
        {
            os << "\t\t\t";
            for (int x = 0; x < 8; x++)
            {
                for (int k = 0; k < 4 - numDigits(data.psqtMG[i * 64 + y * 8 + x]); k++)
                {
                    os << ' ';
                }
                os << data.psqtMG[i * 64 + y * 8 + x] << ", ";
            }
            os << '\n';
        }
        os << "\t\t},\n";
    }
    os << "\t},\n";

    os << "\t{\n";
    for (int i = 0; i < 6; i++)
    {
        os << "\t\t{\n";
        for (int y = 0; y < 8; y++)
        {
            os << "\t\t\t";
            for (int x = 0; x < 8; x++)
            {
                for (int k = 0; k < 4 - numDigits(data.psqtEG[i * 64 + y * 8 + x]); k++)
                {
                    os << ' ';
                }
                os << data.psqtEG[i * 64 + y * 8 + x] << ", ";
            }
            os << '\n';
        }
        os << "\t\t},\n";
    }
    os << "\t},\n";
    os << "\t{" << data.bishopPairMG << ' ' << data.bishopPairEG << "}\n";
    os << "\t{" << data.doubledPawnMG << ' ' << data.doubledPawnEG << "}\n";
    os << "}";
    os << std::endl;
}

int evaluate(const Position& position, const EvalParams& params)
{
    constexpr int WHITE = 0;
    const EvalData& data = params.data;
    int evalMG[2] = {};
    int evalEG[2] = {};

    for (int col = 0; col < 2; col++)
    {
        for (int i = 0; i < position.psqtCount[col]; i++)
        {
            evalMG[col] += params.data.psqtMG[position.psqtIndices[col][i]];
            evalEG[col] += params.data.psqtEG[position.psqtIndices[col][i]];
        }

        if (position.hasBishopPair[col])
        {
            evalMG[col] += params.data.bishopPairMG;
            evalEG[col] += params.data.bishopPairEG;
        }

        evalMG[col] -= params.data.doubledPawnMG * position.doubledPawns[col];
        evalEG[col] -= params.data.doubledPawnEG * position.doubledPawns[col];
    }

    int mg = evalMG[0] - evalMG[1];
    int eg = evalEG[0] - evalEG[1];

    return (mg * (24 - position.phase) + eg * position.phase) / 24;
}