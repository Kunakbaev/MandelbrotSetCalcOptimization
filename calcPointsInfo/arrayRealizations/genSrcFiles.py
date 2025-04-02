
# consts:
MIN_UNROLL_BATCH_SIZE  = 44
MAX_UNROLL_BATCH_SIZE  = 64
UNROLL_BATCH_SIZE_STEP = 4

def createSrcFiles():
    for unrollBatchSize in range(MIN_UNROLL_BATCH_SIZE, MAX_UNROLL_BATCH_SIZE + 1, UNROLL_BATCH_SIZE_STEP):
        fileName = "calcPointsInfo/arrayRealizations/calcPointsInfoArrays" + str(unrollBatchSize) + ".cpp"
        message  = \
f"""#pragma GCC target("avx2")

#include <inttypes.h>
#include <math.h>

#include "../calcPointsInfo.hpp"
#include "../mandelbrotConsts.hpp"

#define functionName calculateMatrixOfPointsInfoArrays{unrollBatchSize}
#define BATCH_SIZE {unrollBatchSize}

#include "calcPointsInfoArraysTemp.hpp"
"""

        with open(fileName, "w") as f:
            f.write(message)

def createMakefileSrc():
    for unrollBatchSize in range(MIN_UNROLL_BATCH_SIZE, MAX_UNROLL_BATCH_SIZE + 1, UNROLL_BATCH_SIZE_STEP):
        path = f"$(CALC_POINTS_INFO_DIR)/arrayRealizations/calcPointsInfoArrays{unrollBatchSize}.cpp             \\"
        print(path)
    print()

def createDeclarations():
    for unrollBatchSize in range(MIN_UNROLL_BATCH_SIZE, MAX_UNROLL_BATCH_SIZE + 1, UNROLL_BATCH_SIZE_STEP):
        path = f"declareArrayRealizationFunc({unrollBatchSize});"
        print(path)
    print()

def main():
    createSrcFiles()
    createMakefileSrc()
    createDeclarations()


if __name__ == "__main__":
    main()
