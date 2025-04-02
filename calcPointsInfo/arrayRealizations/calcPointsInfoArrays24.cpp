#pragma GCC target("avx2")

#include <inttypes.h>
#include <math.h>

#include "../calcPointsInfo.hpp"
#include "../mandelbrotConsts.hpp"

#define functionName calculateMatrixOfPointsInfoArrays24
#define BATCH_SIZE 24

#include "calcPointsInfoArraysTemp.hpp"
