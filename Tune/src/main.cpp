#include "eval_params.h"

#include "pos.h"
#include "optimize.h"
#include "extract.h"

#include <iostream>
#include <fstream>
#include <sstream>


EvalParams defaultParams = {
    {
        {0, 1058, 483, 379, 358, 81},
        {0, 969, 522, 308, 282, 108},
        {
              -9,   -2,  -27,  -83,  -31,  -59,    8,    0,
             -19,   -9,   -5,    3,   -7,    1,    2,    6,
             -14,  -13,    4,    9,   14,   -1,  -15,   -4,
             -14,   10,   12,    4,    6,    2,   17,   -2,
             -33,   -9,    7,   10,    7,   11,   -5,  -20,
             -45,  -13,  -14,   -2,    6,    9,   37,  -14,
        },
        {
             -37,  -13,    2,   15,    6,   16,   -3,  -22,
             -25,  -18,   -3,   -4,   23,   10,   -1,  -20,
               1,    5,    4,    6,   -4,   -2,   -2,  -17,
             -17,  -12,  -11,   -1,   -1,   -5,  -17,  -28,
             -31,  -11,   -5,    5,    3,   -5,  -14,  -35,
              24,   14,    4,   -2,   -6,   -5,   -3,    0,
        },
        {
              26,   -1,  -29,  -21,   18,   80,   83,  109,
              -2,    4,    0,   -7,   -7,   28,   -7,   -9,
              -1,  -21,  -11,  -15,   13,   34,   38,   74,
             -12,    8,   16,   14,    2,   35,   13,  -26,
             -22,  -10,    1,    5,   16,   48,   24, -112,
               0,    5,    0,   -1,    6,   13,   29,    0,
        },
        {
             -42,   -4,    5,    4,    1,   -3,  -12,  -41,
             -61,  -28,  -16,    3,   17,   -4,   14,   10,
              -6,   -4,  -10,   -1,   -4,   -6,    0,   -9,
             -25,   -8,   -5,   -3,    4,   -8,  -21,  -21,
             -35,  -14,   -2,   12,   11,  -11,  -30,  -30,
               0,   -7,  -14,  -17,    0,   69,  148,    0,
        },
    }
};

constexpr int NUM_THREADS = 3;
constexpr double K_VALUE = 3.81492;

int main()
{
	printParams(defaultParams, std::cout);

	// extractMaterial(defaultParams);

	std::vector<ErrorThread> threadPool(NUM_THREADS);

    std::string outFile, inFile;
    std::string str;
    std::getline(std::cin, str);
    std::istringstream stream(str);
    stream >> inFile >> outFile;

    std::ifstream in(inFile);
    std::ofstream out(outFile, std::ios::app);


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

    double kValue = computeKValue(K_VALUE, threadPool, defaultParams, positions);
    // double kValue = K_VALUE;
    std::cout << "Final kValue: " << kValue << std::endl;

    localSearch(kValue, threadPool, defaultParams, positions, out);
}
