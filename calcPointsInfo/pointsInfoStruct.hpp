#ifndef CALC_ESCAPE_TIME_POINT_INFO_STRUCT_HPP
#define CALC_ESCAPE_TIME_POINT_INFO_STRUCT_HPP

#include <cstddef>

struct PointsInfo {
    // that's an array and not a matrix, because it will be easier to work with array when we will apply intrinsics
    float*     lastPointRadiusMatrix;
    size_t*    escTimesMatrix;
};

#endif
