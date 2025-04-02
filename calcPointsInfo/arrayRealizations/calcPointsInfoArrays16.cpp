#pragma GCC target("avx2")

#include <inttypes.h>
#include <math.h>

#include "../calcPointsInfo.hpp"
#include "../mandelbrotConsts.hpp"

#define functionName calculateMatrixOfPointsInfoArrays16
#define BATCH_SIZE 16

#include "calcPointsInfoArraysTemp.hpp"
