#pragma GCC target("avx2")

#include <inttypes.h>
#include <math.h>

#include "../calcPointsInfo.hpp"
#include "../mandelbrotConsts.hpp"

#define functionName calculateMatrixOfPointsInfoArrays56
#define BATCH_SIZE 56

#include "calcPointsInfoArraysTemp.hpp"
