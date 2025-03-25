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

Errors calculateMatrixOfPointsInfo(
    const size_t                    windowHeight,
    const size_t                    windowWidth,
    const PictureParameters*        picParams,
    PointsInfo*                     pointsInfo
) {
    IF_ARG_NULL_RETURN(picParams);
    IF_ARG_NULL_RETURN(pointsInfo);
    IF_ARG_NULL_RETURN(pointsInfo->escTimesMatrix);
    IF_ARG_NULL_RETURN(pointsInfo->lastPointRadiusMatrix);

    // it's faster to access variable, rather than field of a structure which is passed by a pointer
    float scaleFactor = (float)picParams->scaleFactor;
    float picCenterX  = (float)picParams->pictureCenterX;
    float picCenterY  = (float)picParams->pictureCenterY;

    size_t resMatrixInd = 0;
    for (int pixelRow = 0; pixelRow < windowHeight; ++pixelRow) {
        for (int pixelCol = 0; pixelCol < windowWidth; ++pixelCol) {
            float startX = ((float)pixelCol -  windowWidth / 2) / (scaleFactor * windowWidth)  + picCenterX;
            float startY = ((float)pixelRow - windowHeight / 2) / (scaleFactor * windowHeight) + picCenterY;

            float curPointX = startX;
            float curPointY = startY;

            size_t numOfIters = 0;
            float pointRadius = 0;
            for (; numOfIters < MAX_NUM_OF_POINT_ITERATIONS; ++numOfIters) {
                // if (abs(curPointX) > MAX_POINT_COORD ||
                //     abs(curPointY) > MAX_POINT_COORD) break;
                float curXsq = curPointX * curPointX;
                float curYsq = curPointY * curPointY;
                float curXY  = curPointX * curPointY;

                pointRadius = curXsq + curYsq;
                if (pointRadius >= MAX_POINT_RADIUS_SQ) {
                    break;
                }

                curPointX = curXsq - curYsq + startX;
                curPointY = 2 * curXY       + startY;
            }

            pointsInfo->escTimesMatrix       [resMatrixInd] = numOfIters;
            pointsInfo->lastPointRadiusMatrix[resMatrixInd] = pointRadius;
            ++resMatrixInd;
        }
    }

    return STATUS_OK;
}

