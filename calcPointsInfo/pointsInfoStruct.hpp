#ifndef CALC_ESCAPE_TIME_POINT_INFO_STRUCT_HPP
#define CALC_ESCAPE_TIME_POINT_INFO_STRUCT_HPP

#include <cstddef>

struct PointsInfo {
    // that's an array and not a matrix, because it will be easier to work with array when we will apply intrinsics
    float*     lastPointRadiusMatrix;
    int*       escTimesMatrix; // won't need size_t, int is more than enough, also it's size is 8, which is
                               // very convenient as we use intrinsics
};

#endif
