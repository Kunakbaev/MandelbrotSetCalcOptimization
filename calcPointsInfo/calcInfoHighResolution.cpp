#include "calcPointsInfo.hpp"
#include "mandelbrotConsts.hpp"

Errors calculateMatrixOfPointsInfoHighResolution(
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
    long double scaleFactor = picParams->scaleFactor;
    long double picCenterX  = picParams->pictureCenterX;
    long double picCenterY  = picParams->pictureCenterY;

    size_t resMatrixInd = 0;
    for (int pixelRow = 0; pixelRow < windowHeight; ++pixelRow) {
        for (int pixelCol = 0; pixelCol < windowWidth; ++pixelCol) {
            long double startX = ((long double)pixelCol -  windowWidth / 2) / (scaleFactor * windowWidth)  + picCenterX;
            long double startY = ((long double)pixelRow - windowHeight / 2) / (scaleFactor * windowHeight) + picCenterY;

            long double curPointX = startX;
            long double curPointY = startY;

            size_t numOfIters = 0;
            long double pointRadius = 0;
            for (; numOfIters < MAX_NUM_OF_POINT_ITERATIONS; ++numOfIters) {
                long double curXsq = curPointX * curPointX;
                long double curYsq = curPointY * curPointY;
                long double curXY  = curPointX * curPointY;

                pointRadius = curXsq + curYsq;
                if (pointRadius >= (long double)MAX_POINT_RADIUS_SQ) {
                    break;
                }

                curPointX = curXsq - curYsq + startX;
                curPointY = 2 * curXY       + startY;
            }

            pointsInfo->escTimesMatrix       [resMatrixInd] = numOfIters;
            pointsInfo->lastPointRadiusMatrix[resMatrixInd] = (float)pointRadius;
            ++resMatrixInd;
        }
    }

    return STATUS_OK;
}
