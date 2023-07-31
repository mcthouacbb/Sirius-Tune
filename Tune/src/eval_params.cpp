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
    os << "}";
    os << std::endl;
}

int evaluate(const Position& position, const EvalParams& params)
{
    constexpr int FLIP_Y = 0b111000;
    const EvalData& data = params.data;
    int psqtMG[2] = {};
    int psqtEG[2] = {};

    for (int col = 0; col < 2; col++)
    {
        for (int i = 0; i < position.psqtCount[col]; i++)
        {
            psqtMG[col] += params.data.psqtMG[position.psqtIndices[col][i]];
            psqtEG[col] += params.data.psqtEG[position.psqtIndices[col][i]];
        }
    }

    return ((psqtMG[0] - psqtMG[1]) * (24 - position.phase) + (psqtEG[0] - psqtEG[1]) * position.phase) / 24;
}