#pragma once

#include <cstdint>
#include <cassert>
#include <string_view>

enum class PieceType
{
	ALL = 0,
	NONE = 0,
	KING,
	QUEEN,
	ROOK,
	BISHOP,
	KNIGHT,
	PAWN
};

enum class Color
{
	WHITE,
	BLACK
};

inline Color flip(Color c)
{
	return static_cast<Color>(static_cast<int>(c) ^ 1);
}

template<Color c>
inline constexpr Color flip()
{
	return static_cast<Color>(static_cast<int>(c) ^ 1);
}

enum class Piece : uint8_t {};

constexpr Piece PIECE_NONE = Piece(0);

inline Piece makePiece(PieceType type, Color color)
{
	return Piece((static_cast<int>(color) << 3) | static_cast<int>(type));
}

inline PieceType getPieceType(Piece piece)
{
	return static_cast<PieceType>(static_cast<int>(piece) & 0b111);
}

inline Color getPieceColor(Piece piece)
{
	return static_cast<Color>(static_cast<int>(piece) >> 3);
}

class Board
{
public:
	static constexpr const char* defaultFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board();

	Board(const Board&) = delete;
	Board& operator=(const Board&) = delete;

	void setToFen(const std::string_view& fen);
	void setToEpd(const std::string_view& epd);

	Color sideToMove() const;

	Piece getPieceAt(uint32_t square) const;
private:
	void addPiece(uint32_t square, Color c, PieceType p);

	Piece m_Squares[64];

	Color m_SideToMove;
};

inline Piece Board::getPieceAt(uint32_t square) const
{
	return m_Squares[square];
}