#ifndef CALC_POINTS_INFO_MANDELBROT_CONSTS_HPP
#define CALC_POINTS_INFO_MANDELBROT_CONSTS_HPP

#include <cmath>

const float  MAX_POINT_COORD                = powf(2, 10);
const int    MAX_NUM_OF_POINT_ITERATIONS    = 1024;
const float  EPS                            = 1e-3; // TODO:
const float  MAX_POINT_RADIUS_SQ            = powf(10.f, 2);

#endif
