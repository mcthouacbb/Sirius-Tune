#include "eval_params.h"

#include "pos.h"
#include "optimize.h"
#include "extract.h"

#include <iostream>
#include <fstream>
#include <sstream>


EvalParams defaultParams = {
    {
    	{
             148,   82,  101,  101,  118,   96,   63,   61, 
              56,  131,   84,   57,   65,  109,   18,  -15, 
              50,  147,   43,   60,    7,   73,   91,  -10, 
              10,   18,   46,    0,  -19,  -14,    5,  -37, 
              45,   34,    9,  -57,  -46,  -69,  -25,  -72, 
              14,   23,  -67,  -76,  -80,  -56,  -15,  -38, 
              43,  -17,  -27,  -78,  -61,  -37,   13,   11, 
             -49,   23,    3,  -65,    2,  -35,   24,   20, 

             989, 1006, 1050, 1080, 1093, 1110, 1022, 1009, 
             998,  997,  993,  971,  985, 1114, 1049, 1161, 
            1002, 1009, 1035, 1051, 1061, 1104, 1131, 1085, 
             995, 1009, 1016, 1019, 1020, 1028, 1013, 1038, 
            1006, 1014, 1012, 1017, 1028, 1019, 1033, 1013, 
            1007, 1028, 1017, 1024, 1020, 1026, 1030, 1019, 
            1005, 1026, 1030, 1027, 1033, 1046, 1052, 1032, 
            1022, 1008, 1018, 1033, 1012,  998,  974, 1012, 

             524,  526,  543,  543,  584,  594,  543,  518, 
             484,  479,  515,  543,  513,  558,  566,  542, 
             482,  484,  495,  516,  535,  546,  559,  533, 
             451,  471,  495,  510,  491,  501,  512,  505, 
             455,  452,  454,  471,  477,  471,  487,  465, 
             448,  460,  469,  480,  477,  473,  494,  470, 
             441,  464,  461,  469,  472,  477,  486,  421, 
             464,  468,  482,  487,  491,  477,  442,  458, 

             338,  325,  350,  301,  252,  326,  368,  313, 
             338,  376,  352,  362,  376,  422,  371,  394, 
             349,  365,  413,  381,  421,  413,  407,  398, 
             334,  361,  384,  390,  387,  368,  369,  358, 
             351,  371,  372,  390,  396,  374,  374,  340, 
             384,  380,  379,  375,  378,  382,  373,  380, 
             349,  390,  373,  366,  369,  390,  399,  369, 
             367,  346,  360,  351,  338,  346,  362,  353, 

             130,  288,  309,  296,  385,  245,  212,  239, 
             279,  318,  415,  366,  423,  422,  351,  352, 
             341,  366,  371,  404,  433,  462,  412,  350, 
             355,  359,  359,  395,  366,  388,  348,  373, 
             334,  347,  359,  357,  368,  369,  353,  324, 
             324,  339,  360,  357,  367,  355,  365,  324, 
             323,  319,  342,  346,  348,  362,  341,  338, 
             287,  324,  297,  316,  318,  326,  322,  271, 

              82,   82,   82,   82,   82,   82,   82,   82, 
             102,  105,   88,  123,  140,  127,   75,   38, 
              46,   69,   92,   81,  100,  133,   77,   50, 
              62,   81,   75,   86,   99,   81,   81,   47, 
              43,   75,   70,   89,   88,   77,   76,   32, 
              50,   71,   75,   71,   88,   64,  108,   64, 
              47,   83,   64,   60,   75,  102,  123,   65, 
              82,   82,   82,   82,   82,   82,   82,   82, 
        },
        {
             -77,  -33,  -36,  -30,  -29,  -17,  -17,  -45, 
             -28,  -13,  -10,   -7,   -4,    8,   19,    7, 
             -22,   -9,    5,    2,   13,   15,   18,   10, 
             -28,   -3,    5,   16,   18,   25,   16,    7, 
             -41,  -11,    9,   27,   28,   32,   12,    1, 
             -34,  -10,   19,   29,   33,   27,    9,   -3, 
             -36,   -5,   10,   25,   22,   18,    0,  -18, 
             -33,  -31,  -21,  -10,  -36,  -13,  -30,  -56, 

             945,  958,  946,  946,  933,  929,  939,  967, 
             946,  947,  977, 1020, 1004,  971,  976,  849, 
             924,  944,  962,  950,  992,  943,  913,  913, 
             933,  957,  956,  976, 1004,  999,  986,  955, 
             939,  957,  957,  982,  970,  967,  959,  944, 
             918,  908,  966,  946,  967,  959,  960,  929, 
             939,  941,  925,  950,  937,  901,  884,  901, 
             920,  917,  918,  893,  934,  918,  929,  895, 

             517,  513,  510,  508,  495,  487,  498,  504, 
             518,  528,  519,  509,  508,  500,  493,  491, 
             515,  516,  511,  505,  495,  493,  491,  491, 
             517,  511,  512,  504,  506,  505,  494,  494, 
             509,  513,  517,  515,  510,  506,  496,  494, 
             502,  506,  507,  504,  507,  499,  487,  491, 
             506,  505,  512,  511,  507,  503,  498,  509, 
             503,  514,  515,  517,  503,  499,  514,  486, 

             272,  261,  263,  284,  287,  278,  263,  279, 
             267,  278,  280,  272,  281,  265,  278,  243, 
             284,  285,  281,  294,  275,  290,  281,  269, 
             291,  294,  292,  298,  296,  287,  283,  273, 
             275,  283,  296,  290,  290,  297,  275,  266, 
             264,  282,  289,  292,  300,  289,  271,  260, 
             273,  259,  269,  286,  289,  273,  276,  257, 
             252,  269,  256,  278,  279,  268,  256,  262, 

             249,  218,  247,  251,  230,  241,  243,  179, 
             241,  256,  240,  259,  236,  236,  236,  218, 
             241,  254,  284,  279,  253,  255,  244,  243, 
             257,  288,  289,  298,  298,  287,  282,  248, 
             261,  270,  294,  295,  296,  280,  264,  265, 
             256,  273,  274,  287,  285,  277,  261,  242, 
             240,  248,  258,  276,  268,  260,  251,  241, 
             219,  242,  257,  267,  252,  264,  237,  236, 

              94,   94,   94,   94,   94,   94,   94,   94, 
             281,  277,  261,  224,  214,  231,  274,  271, 
             205,  199,  185,  167,  152,  142,  177,  185, 
             131,  120,  109,   95,   90,   98,  111,  115, 
             111,  103,   87,   87,   83,   86,   97,   98, 
             101,  101,   88,   97,   94,   96,   93,   93, 
             113,  104,  108,  108,  104,   98,   96,   93, 
              94,   94,   94,   94,   94,   94,   94,   94, 
    	},
    	17, 47
    }
};

constexpr int NUM_THREADS = 3;
constexpr double K_VALUE = 3.81492;

int main()
{
	printParams(defaultParams, std::cout);

	extractMaterial(defaultParams);

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

    // double kValue = computeKValue(K_VALUE, threadPool, defaultParams, positions);
    double kValue = K_VALUE;
    std::cout << "Final kValue: " << kValue << std::endl;

    localSearch(threadPool, kValue, defaultParams, positions, out);
}
