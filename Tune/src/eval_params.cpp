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
    for (int piece = 0; piece < 6; piece++)
        os << data.materialMG[piece] << ", ";
    os << "}\n\t{";
    for (int piece = 0; piece < 6; piece++)
        os << data.materialEG[piece] << ", ";
    os << "}\n";
        
    os << "\t{\n";
    for (int piece = 0; piece < 6; piece++)
    {
        os << "\t\t{";
        for (int file = 0; file < 8; file++)
        {
            for (int k = 0; k < 4 - numDigits(data.fileMG[piece * 8 + file]); k++)
                os << ' ';
            os << data.fileMG[piece * 8 + file] << ", ";
        }
        os << "},\n";
    }
    os << "\t},\n";

    os << "\t{\n";
    for (int piece = 0; piece < 6; piece++)
    {
        os << "\t\t{";
        for (int file = 0; file < 8; file++)
        {
            for (int k = 0; k < 4 - numDigits(data.fileEG[piece * 8 + file]); k++)
                os << ' ';
            os << data.fileEG[piece * 8 + file] << ", ";
        }
        os << "},\n";
    }
    os << "\t},\n";
    
    os << "\t{\n";
    for (int piece = 0; piece < 6; piece++)
    {
        os << "\t\t{";
        for (int rank = 0; rank < 8; rank++)
        {
            for (int k = 0; k < 4 - numDigits(data.rankMG[piece * 8 + rank]); k++)
                os << ' ';
            os << data.rankMG[piece * 8 + rank] << ", ";
        }
        os << "},\n";
    }
    os << "\t},\n";
    
    os << "\t{\n";
    for (int piece = 0; piece < 6; piece++)
    {
        os << "\t\t{";
        for (int rank = 0; rank < 8; rank++)
        {
            for (int k = 0; k < 4 - numDigits(data.rankEG[piece * 8 + rank]); k++)
                os << ' ';
            os << data.rankEG[piece * 8 + rank] << ", ";
        }
        os << "},\n";
    }
    os << "\t},\n";
    
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
        for (int i = 0; i < position.pieceCount[col]; i++)
        {
            int type = position.pieceTypes[col][i];
            int sq = position.pieceSquares[col][i];
            evalMG[col] += params.data.materialMG[type] + params.data.fileMG[type * 8 + sq % 8] + params.data.rankMG[type * 8 + sq / 8];
            evalEG[col] += params.data.materialEG[type] + params.data.fileEG[type * 8 + sq % 8] + params.data.rankEG[type * 8 + sq / 8];
        }
    }

    int mg = evalMG[0] - evalMG[1];
    int eg = evalEG[0] - evalEG[1];
    
    return (mg * (24 - position.phase) + eg * position.phase) / 24;
}