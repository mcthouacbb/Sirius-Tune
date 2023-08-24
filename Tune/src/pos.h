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
    int pieceCounts[2][6];
    int stmMoveCount;
	double result;
};

std::vector<EpdPos> parseEpdFile(const std::string& str);
std::vector<Position> getPositions(const std::vector<EpdPos>& epds);