#ifndef CALC_POINTS_INFO_CALC_POINTS_INFO_ARRAYS_TEMP_HPP
#define CALC_POINTS_INFO_CALC_POINTS_INFO_ARRAYS_TEMP_HPP

//#pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")
//
// #include <inttypes.h>
// #include <math.h>

#include "../mandelbrotConsts.hpp"
#include "../calcPointsInfo.hpp"

static void set1numberToVector(float* __restrict__ array, const float number) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        array[i] = number;
    }
}

static void add2Vectors(const float* __restrict__ one, const float* __restrict__ two, float* __restrict__ result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] + two[i];
    }
}

static void add2VectorsUnrestricted(const float* one, const float* two, float* result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] + two[i];
    }
}

static void sub2Vectors(const float* __restrict__ one, const float* __restrict__ two, float* __restrict__ result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] - two[i];
    }
}

static void sub2VectorsUnrestricted(const float* one, const float* two, float* result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] - two[i];
    }
}


static void addNum2Vector(const float number, const float* __restrict__ two, float* __restrict__ result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = number + two[i];
    }
}

static void mul2Vectors(const float* one, const float* two, float* result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] * two[i];
    }
}

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
    const float scaleFactor = (float)picParams->scaleFactor;
    const float picCenterX  = (float)picParams->pictureCenterX;
    const float picCenterY  = (float)picParams->pictureCenterY;

    size_t resMatrixInd = 0;
    const float dx = 1.f / (scaleFactor * (float)windowWidth);
    const float dy = 1.f / (scaleFactor * (float)windowHeight);

    const float xOffsetValue =  -(float)windowWidth / 2 * dx + picCenterX;
    const float yOffsetValue = -(float)windowHeight / 2 * dy + picCenterY;
    float xOffset[BATCH_SIZE] = {};
    float dxOffsetReg[BATCH_SIZE] = {};
    for (int i = 0; i < BATCH_SIZE; ++i)
        dxOffsetReg[i] = dx * i;

    float maxPointRadiusSq[BATCH_SIZE] = {};
    set1numberToVector(maxPointRadiusSq, MAX_POINT_RADIUS_SQ);
    set1numberToVector(xOffset, xOffsetValue);

    // with startY += dy error is too big, so I have to use multiplication
    for (int pixelRow = 0; pixelRow < (int)windowHeight; ++pixelRow) {
        float startY[BATCH_SIZE] = {};
        set1numberToVector(startY, (float)pixelRow * dy + yOffsetValue);

        int pixelCol = 0;
        for (; pixelCol + BATCH_SIZE - 1 < (int)windowWidth; pixelCol += BATCH_SIZE) {
            float startX[BATCH_SIZE] = {};
            addNum2Vector((float)pixelCol * dx + xOffsetValue, dxOffsetReg, startX);

            float curPointX[BATCH_SIZE] = {};
            float curPointY[BATCH_SIZE] = {};
            memcpy(curPointX, startX, sizeof(float) * BATCH_SIZE);
            memcpy(curPointY, startY, sizeof(float) * BATCH_SIZE);

            size_t numOfIters = 0;
            uint32_t isBanned = 0;
            int         iters[BATCH_SIZE] = {};
            float      curXsq[BATCH_SIZE] = {};
            float      curYsq[BATCH_SIZE] = {};
            float       curXY[BATCH_SIZE] = {};
            float pointRadius[BATCH_SIZE] = {};

            for (; numOfIters < MAX_NUM_OF_POINT_ITERATIONS; ++numOfIters) {
                mul2Vectors(curPointX, curPointX, curXsq);
                mul2Vectors(curPointY, curPointY, curYsq);
                mul2Vectors(curPointX, curPointY, curXY);

                add2Vectors(curXsq, curYsq, pointRadius);
                int hasAlivePoints = 0;
                for (int i = 0; i < BATCH_SIZE; ++i) {
                    bool f = pointRadius[i] <= maxPointRadiusSq[i];
                    hasAlivePoints += f;
                    iters[i] = iters[i] + f;
                }

                if (!hasAlivePoints)
                    break;

                sub2VectorsUnrestricted(curXsq, curYsq, curPointX);
                add2VectorsUnrestricted(curPointX, startX, curPointX);

                // WARNING:
                add2VectorsUnrestricted(curXY, curXY, curPointY);
                add2VectorsUnrestricted(curPointY, startY, curPointY);
            }

            memcpy(pointsInfo->escTimesMatrix + resMatrixInd, iters, BATCH_SIZE * sizeof(int));
            resMatrixInd += BATCH_SIZE;
        }

        // slow (naive) for loop to deal with unprocessed pixels
        for (; pixelCol < windowWidth; ++pixelCol) {
            float startX = ((float)pixelCol -  windowWidth / 2) / ((float)scaleFactor * windowWidth)  + picCenterX;
            float startY = ((float)pixelRow - windowHeight / 2) / ((float)scaleFactor * windowHeight) + picCenterY;

            float curPointX = startX;
            float curPointY = startY;

            size_t numOfIters = 0;
            float pointRadius = 0;
            for (; numOfIters < MAX_NUM_OF_POINT_ITERATIONS; ++numOfIters) {
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

    //printf("done\n");

    return STATUS_OK;
}

#endif
