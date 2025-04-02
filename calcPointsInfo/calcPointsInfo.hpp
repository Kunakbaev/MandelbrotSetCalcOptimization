#ifndef CALC_ESCAPE_TIME_CALC_ESCAPE_TIME_HPP
#define CALC_ESCAPE_TIME_CALC_ESCAPE_TIME_HPP

#include "../errorsHandler/errorsHandler.hpp"
#include "../graphicalInterface/pictureParamsStruct.hpp"
#include "pointsInfoStruct.hpp"

int getSimulationMaxNumOfIters();

Errors constructPointsInfoStruct(
    PointsInfo*         pointsInfo,
    const size_t        windowHeight,
    const size_t        windowWidth
);

Errors destructPointsInfoStruct(
    PointsInfo*         pointsInfo
);

Errors calculateMatrixOfPointsInfoFloat(
    const size_t                    windowHeight,
    const size_t                    windowWidth,
    const PictureParameters*        picParams,
    PointsInfo*                     pointsInfo
);

Errors calculateMatrixOfPointsInfoOptimizedWithIntrinsics(
    const size_t                    windowHeight,
    const size_t                    windowWidth,
    const PictureParameters*        picParams,
    PointsInfo*                     pointsInfo
);

Errors calculateMatrixOfPointsInfoHighResolution(
    const size_t                    windowHeight,
    const size_t                    windowWidth,
    const PictureParameters*        picParams,
    PointsInfo*                     pointsInfo
);

Errors calculateMatrixOfPointsInfoArrays24(
    const size_t                    windowHeight,
    const size_t                    windowWidth,
    const PictureParameters*        picParams,
    PointsInfo*                     pointsInfo
);

Errors calculateMatrixOfPointsInfoArrays32(
    const size_t                    windowHeight,
    const size_t                    windowWidth,
    const PictureParameters*        picParams,
    PointsInfo*                     pointsInfo
);

#define declareArrayRealizationFunc(unrollBatchSize)                \
    Errors calculateMatrixOfPointsInfoArrays##unrollBatchSize(      \
        const size_t                    windowHeight,               \
        const size_t                    windowWidth,                \
        const PictureParameters*        picParams,                  \
        PointsInfo*                     pointsInfo                  \
    );                                                              \

declareArrayRealizationFunc(16);
declareArrayRealizationFunc(20);
declareArrayRealizationFunc(24);
declareArrayRealizationFunc(28);
declareArrayRealizationFunc(32);
declareArrayRealizationFunc(36);
declareArrayRealizationFunc(40);
declareArrayRealizationFunc(44);
declareArrayRealizationFunc(48);
declareArrayRealizationFunc(52);
declareArrayRealizationFunc(56);
declareArrayRealizationFunc(60);
declareArrayRealizationFunc(64);


#endif
