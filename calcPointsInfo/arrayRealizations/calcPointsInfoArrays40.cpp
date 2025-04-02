#pragma GCC target("avx2")

#include <inttypes.h>
#include <math.h>

#include "../calcPointsInfo.hpp"
#include "../mandelbrotConsts.hpp"

#define functionName calculateMatrixOfPointsInfoArrays40
#define BATCH_SIZE 40

#include "calcPointsInfoArraysTemp.hpp"
