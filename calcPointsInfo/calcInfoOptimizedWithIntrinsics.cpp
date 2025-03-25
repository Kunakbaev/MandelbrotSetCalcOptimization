#pragma GCC target("avx2")

#include <immintrin.h>

#include "mandelbrotConsts.hpp"
#include "calcPointsInfo.hpp"

Errors calculateMatrixOfPointsInfoOptimizedWithIntrinsics(
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
    float dx = 1.0 / (scaleFactor * windowWidth);
    float dy = 1.0 / (scaleFactor * windowHeight);

    float yOffsetValue = - (float)windowHeight / 2 * dy + picCenterY;
    __m256 xOffset = _mm256_set1_ps(-  (float)windowWidth / 2 * dx + picCenterX);
    __m256 yOffset = _mm256_set1_ps(yOffsetValue);
    __m256 dxOffsetReg = _mm256_set_ps(dx * 7, dx * 6, dx * 5, dx * 4, dx * 3, dx * 2, dx, 0);
    __m256 dyOffsetReg = _mm256_set_ps(dy * 7, dy * 6, dy * 5, dy * 4, dy * 3, dy * 2, dy, 0);

    assert(windowWidth % 8 == 0);
    // with startY += dy error is too big, so I have to use multiplication
    for (int pixelRow = 0; pixelRow < windowHeight; ++pixelRow) {
        __m256 startY = _mm256_set1_ps(pixelRow * dy + yOffsetValue);

        for (int pixelCol = 0; pixelCol < windowWidth; pixelCol += 8) {
            //startX = ((float)pixelCol -  windowWidth / 2) / (scaleFactor * windowWidth)  + picCenterX;
            //startY = ((float)pixelRow - windowHeight / 2) * dy + picCenterY;
            //startX = (float)pixelCol * dx -  (float)windowWidth / 2 * dx  + picCenterX;
            __m256 startX = _mm256_add_ps(_mm256_add_ps(_mm256_set1_ps(pixelCol * dx), dxOffsetReg), xOffset);
            //startY = (float)pixelRow * dy - (float)windowHeight / 2 * dy + picCenterY;


            // float curPointX = startX;
            // float curPointY = startY;
            __m256 curPointX = startX;
            __m256 curPointY = startY;

            size_t numOfIters = 0;
            __m256 pointRadius;
            __m256i iters = _mm256_set1_epi32(0);
            for (; numOfIters < MAX_NUM_OF_POINT_ITERATIONS; ++numOfIters) {
                // if (abs(curPointX) > MAX_POINT_COORD ||
                //     abs(curPointY) > MAX_POINT_COORD) break;
                __m256 curXsq = _mm256_mul_ps(curPointX, curPointX);
                __m256 curYsq = _mm256_mul_ps(curPointY, curPointY);
                __m256 curXY  = _mm256_mul_ps(curPointX, curPointY);

                //pointRadius = curXsq + curYsq;
                pointRadius = _mm256_add_ps(curXsq, curYsq);
                __m256 cmp  = _mm256_cmp_ps(pointRadius, _mm256_set1_ps(MAX_POINT_RADIUS_SQ), _CMP_LE_OQ);
                int mask = _mm256_movemask_ps(cmp);
                if (!mask) { // all 8 points are out of radius
                    break;
                }

                iters = _mm256_sub_epi32(iters, _mm256_castps_si256(cmp));

                // curPointX = curXsq - curYsq + startX;
                // curPointY = 2 * curXY       + startY;

                // for smooth coloring
                __m256 tmp = _mm256_add_ps(_mm256_sub_ps(curXsq, curYsq), startX);
                curPointX = _mm256_blendv_ps(curPointX, tmp, cmp);

                __m256 tmp2 = _mm256_add_ps(_mm256_add_ps(curXY,   curXY), startY);
                curPointY = _mm256_blendv_ps(curPointY, tmp2, cmp);

                // for simple coloring
                // curPointX = _mm256_add_ps(_mm256_sub_ps(curXsq, curYsq), startX);
                // curPointY = _mm256_add_ps(_mm256_add_ps(curXY,   curXY), startY);

                // curPointX = _mm256_blendv_ps(_mm256_setzero_ps(), curPointX, cmp);
                // curPointY = _mm256_blendv_ps(_mm256_setzero_ps(), curPointY, cmp);
                // cmp = _mm256_castsi256_ps(_mm256_set1_epi32(
                // curPointX = _mm256_and_ps(curPointX, cmp);
                // curPointY = _mm256_and_ps(curPointY, cmp);
            }

            //int array[8] = {};
            _mm256_storeu_si256((__m256i*)(pointsInfo->escTimesMatrix + resMatrixInd), iters);
            //_mm256_storeu_si256((__m256i*)array, iters);
            _mm256_storeu_ps((pointsInfo->lastPointRadiusMatrix + resMatrixInd), pointRadius);
            //memcpy(pointsInfo->escTimesMatrix + resMatrixInd, array, 8 * sizeof(int));
            // for (int i = 0; i < 8; ++i)
            //     *(pointsInfo->escTimesMatrix + resMatrixInd) = array[i];
            // LOG_DEBUG_VARS(pointsInfo->escTimesMatrix[resMatrixInd]);
            // LOG_DEBUG_VARS(pointsInfo->lastPointRadiusMatrix[resMatrixInd]);
            // LOG_DEBUG_VARS(array[0]);
            // pointsInfo->escTimesMatrix       [resMatrixInd] = numOfIters;
            // pointsInfo->lastPointRadiusMatrix[resMatrixInd] = pointRadius;
            resMatrixInd += 8;
        }
    }

    return STATUS_OK;
}
