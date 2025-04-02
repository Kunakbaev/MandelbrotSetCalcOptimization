#pragma GCC target("avx2")

#include <inttypes.h>
#include <math.h>

#include "../calcPointsInfo.hpp"
#include "../mandelbrotConsts.hpp"

#define functionName calculateMatrixOfPointsInfoArrays60
#define BATCH_SIZE 60

#include "calcPointsInfoArraysTemp.hpp"
