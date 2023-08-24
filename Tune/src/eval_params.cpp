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
    os << "\t{";
    for (int i = 0; i < 6; i++)
    {
        for (int i = 0; i < 4 - numDigits(data.material[i]); i++)
            os << ' ';
        os << data.material[i] << (i != 5 ? ", " : "");
    }
    os << "},\n";
    os << '\t' << data.moveCount;
    os << "\n}";
    os << std::endl;
}

int evaluate(const Position& position, const EvalParams& params)
{
    const EvalData& data = params.data;
    int material[2] = {};

    for (int col = 0; col < 2; col++)
    {
        for (int pce = 0; pce < 6; pce++)
        {
            material[col] += position.pieceCounts[col][pce] * data.material[pce];
        }
    }

    int stmMoveCount = data.moveCount * position.stmMoveCount / 100;

    return material[0] - material[1] + stmMoveCount;
}