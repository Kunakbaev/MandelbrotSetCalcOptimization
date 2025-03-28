#ifndef CALC_POINTS_INFO_COMMON_CALC_INFO_TEMPLATE_HPP
#define CALC_POINTS_INFO_COMMON_CALC_INFO_TEMPLATE_HPP

#include "calcPointsInfo.hpp"
#include "mandelbrotConsts.hpp"

// this file will be included twice, before include operandType will be defined (float or long double) and
// function name

// #define functionName calcPointsInfo
// #define operandType  float

Errors functionName (
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
    operandType scaleFactor = (operandType)picParams->scaleFactor;
    operandType picCenterX  = (operandType)picParams->pictureCenterX;
    operandType picCenterY  = (operandType)picParams->pictureCenterY;

    size_t resMatrixInd = 0;
    for (int pixelRow = 0; pixelRow < windowHeight; ++pixelRow) {
        for (int pixelCol = 0; pixelCol < windowWidth; ++pixelCol) {
            operandType startX = ((operandType)pixelCol -  windowWidth / 2) / ((operandType)scaleFactor * windowWidth)  + picCenterX;
            operandType startY = ((operandType)pixelRow - windowHeight / 2) / ((operandType)scaleFactor * windowHeight) + picCenterY;

            operandType curPointX = startX;
            operandType curPointY = startY;

            size_t numOfIters = 0;
            operandType pointRadius = 0;
            for (; numOfIters < MAX_NUM_OF_POINT_ITERATIONS; ++numOfIters) {
                // if (abs(curPointX) > MAX_POINT_COORD ||
                //     abs(curPointY) > MAX_POINT_COORD) break;
                operandType curXsq = curPointX * curPointX;
                operandType curYsq = curPointY * curPointY;
                operandType curXY  = curPointX * curPointY;

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

#endif
