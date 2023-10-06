#pragma once

#include <vector>
#include <string>

struct EpdPos
{
	const char* epd;
	int epdLen;
	double result;
};

struct Position
{
	int pieceTypes[2][16];
    int pieceSquares[2][16];
	int pieceCount[2];
	int phase;
	double result;
};

std::vector<EpdPos> parseEpdFile(const std::string& str);
std::vector<Position> getPositions(const std::vector<EpdPos>& epds);