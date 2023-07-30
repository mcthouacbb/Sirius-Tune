#pragma once

#include <vector>
#include <string>

struct Pos
{
	const char* epd;
	int epdLen;
	double result;
};

std::vector<Pos> parseEpdFile(const std::string& str);