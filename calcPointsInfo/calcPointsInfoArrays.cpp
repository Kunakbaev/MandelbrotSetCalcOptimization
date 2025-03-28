//#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")

#include <inttypes.h>
#include <math.h>

#include "mandelbrotConsts.hpp"
#include "calcPointsInfo.hpp"

const int BATCH_SIZE = 32;

void set1numberToVector(float* __restrict__ array, const float number) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        array[i] = number;
    }
}

void add2Vectors(const float* __restrict__ one, const float* __restrict__ two, float* __restrict__ result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] + two[i];
    }

    // result[0] = one[0] + two[0];
    // result[1] = one[1] + two[1];
    // result[2] = one[2] + two[2];
    // result[3] = one[3] + two[3];
    // result[4] = one[4] + two[4];
    // result[5] = one[5] + two[5];
    // result[6] = one[6] + two[6];
    // result[7] = one[7] + two[7];
}

void add2VectorsUnrestricted(const float* one, const float* two, float* result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] + two[i];
    }

    // result[0] = one[0] + two[0];
    // result[1] = one[1] + two[1];
    // result[2] = one[2] + two[2];
    // result[3] = one[3] + two[3];
    // result[4] = one[4] + two[4];
    // result[5] = one[5] + two[5];
    // result[6] = one[6] + two[6];
    // result[7] = one[7] + two[7];
}

void sub2Vectors(const float* __restrict__ one, const float* __restrict__ two, float* __restrict__ result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] - two[i];
    }

    // result[0] = one[0] - two[0];
    // result[1] = one[1] - two[1];
    // result[2] = one[2] - two[2];
    // result[3] = one[3] - two[3];
    // result[4] = one[4] - two[4];
    // result[5] = one[5] - two[5];
    // result[6] = one[6] - two[6];
    // result[7] = one[7] - two[7];
}

void sub2VectorsUnrestricted(const float* one, const float* two, float* result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] - two[i];
    }

    // result[0] = one[0] - two[0];
    // result[1] = one[1] - two[1];
    // result[2] = one[2] - two[2];
    // result[3] = one[3] - two[3];
    // result[4] = one[4] - two[4];
    // result[5] = one[5] - two[5];
    // result[6] = one[6] - two[6];
    // result[7] = one[7] - two[7];
}


void addNum2Vector(const float number, const float* __restrict__ two, float* __restrict__ result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = number + two[i];
    }

    // result[0] = number + two[0];
    // result[1] = number + two[1];
    // result[2] = number + two[2];
    // result[3] = number + two[3];
    // result[4] = number + two[4];
    // result[5] = number + two[5];
    // result[6] = number + two[6];
    // result[7] = number + two[7];
}

void mul2Vectors(const float* one, const float* two, float* result) {
    for (int i = 0; i < BATCH_SIZE; ++i) {
        result[i] = one[i] * two[i];
    }

    // result[0] = one[0] * two[0];
    // result[1] = one[1] * two[1];
    // result[2] = one[2] * two[2];
    // result[3] = one[3] * two[3];
    // result[4] = one[4] * two[4];
    // result[5] = one[5] * two[5];
    // result[6] = one[6] * two[6];
    // result[7] = one[7] * two[7];
}

Errors calculateMatrixOfPointsInfoArrays(
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
    //const float dxOffsetReg[] = {dx * 7, dx * 6, dx * 5, dx * 4, dx * 3, dx * 2, dx, 0};
    //const float dxOffsetReg[] = {0, dx, dx * 2, dx * 3, dx * 4, dx * 5, dx * 6, dx * 7};
    float dxOffsetReg[BATCH_SIZE] = {};
    for (int i = 0; i < BATCH_SIZE; ++i)
        dxOffsetReg[i] = dx * i;

    float maxPointRadiusSq[BATCH_SIZE] = {};
    set1numberToVector(maxPointRadiusSq, MAX_POINT_RADIUS_SQ);
    set1numberToVector(xOffset, xOffsetValue);

    //printf("start\n");
    //assert(windowWidth % BATCH_SIZE == 0);
    // with startY += dy error is too big, so I have to use multiplication
    for (int pixelRow = 0; pixelRow < (int)windowHeight; ++pixelRow) {
        float startY[BATCH_SIZE] = {};
        set1numberToVector(startY, (float)pixelRow * dy + yOffsetValue);

        for (int pixelCol = 0; pixelCol < (int)windowWidth; pixelCol += BATCH_SIZE) {
            //startX = ((float)pixelCol -  windowWidth / 2) / (scaleFactor * windowWidth)  + picCenterX;
            //startY = ((float)pixelRow - windowHeight / 2) * dy + picCenterY;
            //startX = (float)pixelCol * dx -  (float)windowWidth / 2 * dx  + picCenterX;

            // TODO: rewrite with += dx
            float startX[BATCH_SIZE] = {};
            addNum2Vector((float)pixelCol * dx + xOffsetValue, dxOffsetReg, startX);
            //startY = (float)pixelRow * dy - (float)windowHeight / 2 * dy + picCenterY;

            // float curPointX = startX;
            // float curPointY = startY;
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

                //pointRadius = curXsq + curYsq;
                add2Vectors(curXsq, curYsq, pointRadius);
                int hasAlivePoints = 0;
                for (int i = 0; i < BATCH_SIZE; ++i) {
                    // if (pointRadius[i] <= maxPointRadiusSq[i]) {
                    //     hasAlivePoints += 1;
                    //     iters[i] += 1;
                    // }

                    bool f = pointRadius[i] <= maxPointRadiusSq[i];
                    //printf("flag : %d\n", f);
                    hasAlivePoints += f;
                    iters[i] += f;
                }

                if (!hasAlivePoints)
                    break;

                // curPointX = curXsq - curYsq + startX;
                // curPointY = 2 * curXY       + startY;

                // for smooth coloring
                // blendv is really slow
                // __m256 tmp = _mm256_add_ps(_mm256_sub_ps(curXsq, curYsq), startX);
                // tmp = _mm256_sub_ps(tmp, curPointX);
                // tmp = _mm256_and_ps(tmp, cmp);
                // curPointX = _mm256_add_ps(curPointX, tmp);
                //curPointX = _mm256_blendv_ps(curPointX, tmp, cmp);

                // __m256 tmp2 = _mm256_add_ps(_mm256_add_ps(curXY,   curXY), startY);
                // tmp2 = _mm256_sub_ps(tmp2, curPointY);
                // tmp2 = _mm256_and_ps(tmp2, cmp);
                // curPointY = _mm256_add_ps(curPointY, tmp2);
                //curPointY = _mm256_blendv_ps(curPointY, tmp2, cmp);

                // add difference * cmp mask
                // agner fog cpu instruction
                //curPointY = _mm256_blend_ps(curPointY, tmp2, mask);

                // // for simple coloring
                sub2VectorsUnrestricted(curXsq, curYsq, curPointX);
                add2VectorsUnrestricted(curPointX, startX, curPointX);

                //curPointX = _mm256_add_ps(_mm256_sub_ps(curXsq, curYsq), startX);
                //curPointY = _mm256_add_ps(_mm256_add_ps(curXY,   curXY), startY);

                // WARNING:
                add2VectorsUnrestricted(curXY, curXY, curPointY);
                add2VectorsUnrestricted(curPointY, startY, curPointY);
            }

            //printf("biba\n");
            // printf("iters : ");
            // for (int i = 0; i < BATCH_SIZE; ++i)
            //     printf("%d ", iters[i]);
            // printf("\n");
            memcpy(pointsInfo->escTimesMatrix + resMatrixInd, iters, BATCH_SIZE * sizeof(int));
            //_mm256_storeu_si256((__m256i*)(pointsInfo->escTimesMatrix + resMatrixInd), iters);
            //_mm256_storeu_ps((pointsInfo->lastPointRadiusMatrix + resMatrixInd), pointRadius);
            resMatrixInd += BATCH_SIZE;
        }
    }

    //printf("done\n");

    return STATUS_OK;
}
