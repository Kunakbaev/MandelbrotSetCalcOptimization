#include <cmath>
#include <immintrin.h>

#include "calcPointsInfo.hpp"
#include "mandelbrotConsts.hpp"

Errors constructPointsInfoStruct(
    PointsInfo*         pointsInfo,
    const size_t        windowHeight,
    const size_t        windowWidth
) {
    IF_ARG_NULL_RETURN(pointsInfo);

    size_t numOfElementsInArr = windowHeight * windowWidth;
    pointsInfo->escTimesMatrix        =   (int*)calloc(numOfElementsInArr, sizeof(int));
    pointsInfo->lastPointRadiusMatrix = (float*)calloc(numOfElementsInArr, sizeof(float));
    IF_NOT_CONDITION_RETURN(pointsInfo->escTimesMatrix        != NULL, MEMORY_ALLOCATION_ERROR);
    IF_NOT_CONDITION_RETURN(pointsInfo->lastPointRadiusMatrix != NULL, MEMORY_ALLOCATION_ERROR);

    return STATUS_OK;
}

int getSimulationMaxNumOfIters() {
    return MAX_NUM_OF_POINT_ITERATIONS;
}

Errors destructPointsInfoStruct(
    PointsInfo*         pointsInfo
) {
    IF_ARG_NULL_RETURN(pointsInfo);

    free(pointsInfo->escTimesMatrix);
    free(pointsInfo->lastPointRadiusMatrix);

    return STATUS_OK;
}
