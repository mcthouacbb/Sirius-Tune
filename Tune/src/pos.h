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
	int psqtIndices[2][16];
	int psqtCount[2];
    bool hasBishopPair[2];
	int phase;
	double result;
};

std::vector<EpdPos> parseEpdFile(const std::string& str);
std::vector<Position> getPositions(const std::vector<EpdPos>& epds);