#ifndef CALC_ESCAPE_TIME_CALC_ESCAPE_TIME_HPP
#define CALC_ESCAPE_TIME_CALC_ESCAPE_TIME_HPP

#include "../errorsHandler/errorsHandler.hpp"
#include "../graphicalInterface/pictureParamsStruct.hpp"
#include "pointsInfoStruct.hpp"

Errors constructPointsInfoStruct(
    PointsInfo*         pointsInfo,
    const size_t        windowHeight,
    const size_t        windowWidth
);

Errors calculateMatrixOfPointsInfo(
    const size_t                    windowHeight,
    const size_t                    windowWidth,
    const PictureParameters*        picParams,
    PointsInfo*                     pointsInfo
);

#endif
