#include "pos.h"
#include "board.h"

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

inline int getPhase(PieceType pce)
{
	if (pce == PieceType::QUEEN)
		return 4;
	else if (pce == PieceType::ROOK)
		return 2;
	else if (pce == PieceType::KNIGHT || pce == PieceType::BISHOP)
		return 1;
	return 0;
}

std::vector<Position> getPositions(const std::vector<EpdPos>& epds)
{
	std::vector<Position> positions;

	Board board;
	for (const auto& epd : epds)
	{
		board.setToEpd(std::string_view(epd.epd, epd.epdLen));
		Position pos = {};
		pos.phase = 24;
		pos.result = epd.result;

		for (int sq = 0; sq < 64; sq++)
		{
			Piece pce = board.getPieceAt(sq);

			if (pce == PIECE_NONE)
				continue;

			PieceType type = getPieceType(pce);
			int color = static_cast<int>(getPieceColor(pce));

			pos.phase -= getPhase(type);
			int psqtIdx = pos.psqtCount[color]++;
			pos.psqtIndices[color][psqtIdx] = 64 * static_cast<int>(type) - 64 + (sq ^ (color == static_cast<int>(Color::WHITE) ? 0b111000 : 0));
		}

		positions.push_back(pos);
	}

	return positions;
}