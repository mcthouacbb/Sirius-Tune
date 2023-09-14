#include "pos.h"
#include "chess/chess.hpp"

#include <stdexcept>

std::vector<EpdPos> parseEpdFile(const std::string& str)
{
	std::vector<EpdPos> positions;

	int lineStart = 0;
	int lineEnd = static_cast<int>(str.find('\n', 0));

	do
	{
		if (str[lineStart] == '\0')
			break;
		double result;
		int lastChar = lineEnd == static_cast<int>(std::string::npos) ? static_cast<int>(str.size() - 1) : lineEnd - 1;
		int len = lineEnd - lineStart;
		if (str[lastChar] == '0')
		{
			result = 1;
			len -= 3;
		}
		else if (str[lastChar] == '2')
		{
			result = 0.5;
			len -= 7;
		}
		else if (str[lastChar] == '1')
		{
			result = 0;
			len -= 3;
		}
		else
		{
			/*std::cout << str[lastChar] << std::endl;
			std::cout << lineEnd << std::endl;
			std::cout.write(&str[lastChar - 10], 50) << std::endl;*/
			throw std::runtime_error("tune::parseEpdFile(): Unexpected character in ");
		}
		positions.push_back({str.c_str() + lineStart, len, result});
		lineStart = lineEnd + 1;
		lineEnd = static_cast<int>(str.find('\n', lineStart));
	} while (lineEnd != static_cast<int>(std::string::npos));

	return positions;
}

inline int getPhase(chess::PieceType pce)
{
	if (pce == chess::PieceType::QUEEN)
		return 4;
	else if (pce == chess::PieceType::ROOK)
		return 2;
	else if (pce == chess::PieceType::KNIGHT || pce == chess::PieceType::BISHOP)
		return 1;
	return 0;
}

namespace
{
    int getPieceNum(chess::PieceType pieceType)
    {
        switch (pieceType)
        {
            case chess::PieceType::PAWN:
                return 6;
            case chess::PieceType::KNIGHT:
                return 5;
            case chess::PieceType::BISHOP:
                return 4;
            case chess::PieceType::ROOK:
                return 3;
            case chess::PieceType::QUEEN:
                return 2;
            case chess::PieceType::KING:
                return 1;
        }
        return -1;
    }

    int getColorNum(chess::Color color)
    {
        if (color == chess::Color::WHITE)
            return 0;
        else
            return 1;
    }
}

std::vector<Position> getPositions(const std::vector<EpdPos>& epds)
{
    using chess::builtin::popcount;
	std::vector<Position> positions;

	constexpr uint64_t LIGHT_SQUARES = 0x55AA55AA55AA55AAull;
	constexpr uint64_t DARK_SQUARES = 0xAA55AA55AA55AA55ull;

	chess::Board board;
	for (const auto& epd : epds)
	{
		board.setFen(std::string(epd.epd, epd.epdLen));
		Position pos = {};
		pos.phase = 24;
		pos.result = epd.result;

		for (int sq = 0; sq < 64; sq++)
		{
			chess::Piece pce = board.at(static_cast<chess::Square>(sq));

			if (pce == chess::Piece::NONE)
				continue;

			chess::PieceType type = chess::utils::typeOfPiece(pce);
			chess::Color color = chess::Board::color(pce);

			pos.phase -= getPhase(type);
			int psqtIdx = pos.psqtCount[getColorNum(color)]++;
			pos.psqtIndices[getColorNum(color)][psqtIdx] = 64 * getPieceNum(type) - 64 + (sq ^ (color == chess::Color::WHITE ? 0b111000 : 0));
		}


        uint64_t whiteBishops = board.pieces(chess::PieceType::BISHOP, chess::Color::WHITE);
        uint64_t blackBishops = board.pieces(chess::PieceType::BISHOP, chess::Color::BLACK);
        if ((whiteBishops & LIGHT_SQUARES) != 0 && (whiteBishops & DARK_SQUARES) != 0)
            pos.hasBishopPair[0] = true;
        if ((blackBishops & LIGHT_SQUARES) != 0 && (blackBishops & DARK_SQUARES) != 0)
            pos.hasBishopPair[1] = true;

		positions.push_back(pos);
	}

	return positions;
}