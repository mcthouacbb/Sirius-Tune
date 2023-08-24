#include "eval_params.h"

#include "pos.h"
#include "optimize.h"
#include "extract.h"

#include <iostream>
#include <fstream>
#include <sstream>



EvalParams defaultParams = {
    {
        {0, 977, 492, 335, 318, 98},
        25
    }
};

constexpr int NUM_THREADS = 3;

int main()
{
	printParams(defaultParams, std::cout);

	std::vector<ErrorThread> threadPool(NUM_THREADS);

    std::string outFile, inFile;
    std::string str;
    std::getline(std::cin, str);
    std::istringstream stream(str);
    stream >> inFile >> outFile;

    std::ifstream in(inFile);
    std::ofstream out(outFile);


	std::vector<Position> positions;
    {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        std::string epdString(size, '\0');
        in.seekg(0);
        in.read(epdString.data(), size);

        std::vector<EpdPos> epds = parseEpdFile(epdString);
        positions = getPositions(epds);
    }

    double kValue = computeKValue(threadPool, defaultParams, positions);
    std::cout << "Final kValue: " << kValue;

    localSearch(threadPool, kValue, defaultParams, positions, out);
}
