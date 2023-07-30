#include "board.h"

Board::Board()
{
	setToFen(defaultFen);
}

void Board::setToFen(const std::string_view& fen)
{
	setToEpd(fen);
}

void Board::setToEpd(const std::string_view& epd)
{
	memset(m_Squares, 0, 64);
	int i = 0;
	int sq = 56;
	for (;; i++)
	{
		switch (epd[i])
		{
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				sq += epd[i] - '0';
				break;
			case 'k':
				addPiece(sq++, Color::BLACK, PieceType::KING);
				break;
			case 'q':
				addPiece(sq++, Color::BLACK, PieceType::QUEEN);
				break;
			case 'r':
				addPiece(sq++, Color::BLACK, PieceType::ROOK);
				break;
			case 'b':
				addPiece(sq++, Color::BLACK, PieceType::BISHOP);
				break;
			case 'n':
				addPiece(sq++, Color::BLACK, PieceType::KNIGHT);
				break;
			case 'p':
				addPiece(sq++, Color::BLACK, PieceType::PAWN);
				break;
			case 'K':
				addPiece(sq++, Color::WHITE, PieceType::KING);
				break;
			case 'Q':
				addPiece(sq++, Color::WHITE, PieceType::QUEEN);
				break;
			case 'R':
				addPiece(sq++, Color::WHITE, PieceType::ROOK);
				break;
			case 'B':
				addPiece(sq++, Color::WHITE, PieceType::BISHOP);
				break;
			case 'N':
				addPiece(sq++, Color::WHITE, PieceType::KNIGHT);
				break;
			case 'P':
				addPiece(sq++, Color::WHITE, PieceType::PAWN);
				break;
			case '/':
				sq -= 16;
				break;
			default:
				goto done;
		}
	}
done:
	i++;
	m_SideToMove = epd[i] == 'w' ? Color::WHITE : Color::BLACK;
}


void Board::addPiece(uint32_t square, Color c, PieceType p)
{
	m_Squares[square] = makePiece(p, c);
}