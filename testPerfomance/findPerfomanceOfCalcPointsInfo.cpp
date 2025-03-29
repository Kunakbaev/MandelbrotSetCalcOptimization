#include <stdio.h>
#include <cmath>
#include <chrono>

#include "../errorsHandler/errorsHandler.hpp"
#include "../calcPointsInfo/calcPointsInfo.hpp"

const int WINDOW_WIDTH      = 1024;
const int WINDOW_HEIGHT     = 1024;
const int NUM_OF_ITERATIONS = 10;

float calcTimeArray[NUM_OF_ITERATIONS] = {};

typedef Errors (*calcPointsInfoFuncPtr)(
    const size_t                    windowHeight,
    const size_t                    windowWidth,
    const PictureParameters*        picParams,
    PointsInfo*                     pointsInfo
);

struct PerfomanceAlgo {
    const char*           name;
    calcPointsInfoFuncPtr func;
};

PerfomanceAlgo algos[] = {
    {.name = "highRes",         calculateMatrixOfPointsInfoHighResolution          },
    {.name = "float",           calculateMatrixOfPointsInfoFloat                   },
    {.name = "floatIntrinsics", calculateMatrixOfPointsInfoOptimizedWithIntrinsics },
    {.name = "floatArr",        calculateMatrixOfPointsInfoArrays                  },
};

const size_t numOfPerfAlgos = sizeof(algos) / sizeof(*algos);

Errors getCalcFunction(
    int argc,
    const char** argv,
    calcPointsInfoFuncPtr* funcPtr
) {
    IF_NOT_CONDITION_RETURN(argc == 2, INCORRENT_NUM_OF_ARGS_FOR_PROGRAMM_ERROR);
    IF_ARG_NULL_RETURN(argv);
    IF_ARG_NULL_RETURN(funcPtr);
    IF_ARG_NULL_RETURN(argv[1]);

    const char* perfAlgoName = argv[1];
    LOG_DEBUG_VARS(perfAlgoName);
    for (size_t i = 0; i < numOfPerfAlgos; ++i) {
        int cmpRes = strcmp(perfAlgoName, algos[i].name);
        LOG_DEBUG_VARS(algos[i].name);
        if (cmpRes == 0) {
            *funcPtr = algos[i].func;
            return STATUS_OK;
        }
    }

    LOG_ERROR(getErrorMessage(PERFOMANCE_ALGO_NOT_FOUND_ERROR));
    assert(false);
    return PERFOMANCE_ALGO_NOT_FOUND_ERROR;
}

int main(int argc, const char** argv) {
    setLoggingLevel(DEBUG);

    calcPointsInfoFuncPtr funcPtr = NULL;
    IF_ERR_RETURN(getCalcFunction(argc, argv, &funcPtr));

    PictureParameters pictureParams = {
        .pictureCenterX = 0,
        .pictureCenterY = 0,
        .scaleFactor    = 1.0,
    };

    PointsInfo pointsInfo = {};
    IF_ERR_RETURN(constructPointsInfoStruct(&pointsInfo, WINDOW_WIDTH, WINDOW_HEIGHT));

    float meanTime = 0.f;
    float dispersion = 0.f;
    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; ++iteration) {
        auto start = std::chrono::steady_clock::now().time_since_epoch().count();
        (*funcPtr)(WINDOW_HEIGHT, WINDOW_WIDTH, &pictureParams, &pointsInfo);
        auto end = std::chrono::steady_clock::now().time_since_epoch().count();
        // in milliseconds
        float calcTime = (float)(end - start) / CLOCKS_PER_SEC;
        calcTimeArray[iteration] = calcTime;

        LOG_DEBUG_VARS(iteration, calcTime);
        meanTime += calcTime;
    }
    meanTime /= NUM_OF_ITERATIONS;

    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; ++iteration) {
        float meanSquaredError = calcTimeArray[iteration] - meanTime;
        meanSquaredError *= meanSquaredError;
        dispersion += meanSquaredError;
        //LOG_DEBUG_VARS(meanSquaredError);
    }
    dispersion /= NUM_OF_ITERATIONS;

    assert(dispersion >= 0);
    float meanSquareError = sqrt(dispersion);

    float meanTimeOfGoodVals = 0;
    int cntGood = 0;
    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; ++iteration) {
        float delta = fabsf(calcTimeArray[iteration] - meanTime);
        if (delta < meanSquareError) {
            //LOG_DEBUG_VARS(delta, calcTimeArray[iteration]);
            meanTimeOfGoodVals += calcTimeArray[iteration];
            ++cntGood;
        }
    }
    meanTimeOfGoodVals /= cntGood;

    printf("mean time : %f, dispersion : %f\n", meanTime, dispersion);
    //printf("sigma : %f, 2 * sigma : %f\n", meanSquareError, 2 * meanSquareError);
    printf("sigma : %f\n", meanSquareError);
    printf("validated mean time: %f\n", meanTimeOfGoodVals);

    IF_ERR_RETURN(destructPointsInfoStruct(&pointsInfo));

    return 0;
}

/*

-Ofast
float :                 2431.330322         19.16
floatIntrinsics:        314.371002          8.383911
floatArr:               206.541214          7.928277

-O3
float :                 2546.723877         8.028677
floatIntrinsics:        329.517426          6.447448
floatArr:               213.129440          4.512328

-O2
float :                 2544.549561         49.054939
floatIntrinsics:        333.004608          5.936242
floatArr:               3592.113281         53.228645

-O1
float :                 2480.524658         64.678436
floatIntrinsics:        328.854340          2.331454
floatArr:               3064.068848         28.810362

no optimization flags
float :                 4918.506836         36.559334
floatIntrinsics:        2724.700928         4.805051
floatArr:               16347.489258        210.500427

*/
