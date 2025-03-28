#include <stdio.h>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "errorsHandler/errorsHandler.hpp"
#include "graphicalInterface/graphicalInterface.hpp"
#include "calcPointsInfo/calcPointsInfo.hpp"

const int   WINDOW_WIDTH        = 800;
const int   WINDOW_HEIGHT       = 800;
const float FPS_UPDATE_INTERVAL = 200; // in milliseconds

int main() {
    setLoggingLevel(DEBUG);
    LOG_DEBUG("Hello world!");

    precountGradientColorsArray();

    PointsInfo pointsInfo = {};
    IF_ERR_RETURN(constructPointsInfoStruct(&pointsInfo, WINDOW_WIDTH, WINDOW_HEIGHT));

    sf::VideoMode mode(WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::RenderWindow window(mode, "i am window");

    GraphicalInterface graphInt = {};
    IF_ERR_RETURN(constructGraphicalInterface(&graphInt, &window, WINDOW_WIDTH, WINDOW_HEIGHT));
    showGradient(graphInt.window);

    float fps = 30.f;
    auto lastTimeOfFpsUpdate = std::chrono::high_resolution_clock::now();
    while (isWindowOpen(&graphInt)) {
        IF_ERR_RETURN(windowEventsLoop(&graphInt));
        IF_ERR_RETURN(closeWindowIfQuitKeyPressed(&graphInt));

        auto start = std::chrono::high_resolution_clock::now();
        window.clear();
        IF_ERR_RETURN(pictureParametresUpdate(&graphInt, fps));

        // TODO:
        if (graphInt.wasPictureUpdate | 1) {
            //calculateMatrixOfPointsInfoFloat(WINDOW_HEIGHT, WINDOW_WIDTH, &graphInt.pictureParams, &pointsInfo);
            //calculateMatrixOfPointsInfoOptimizedWithIntrinsics(WINDOW_HEIGHT, WINDOW_WIDTH, &graphInt.pictureParams, &pointsInfo);
            //calculateMatrixOfPointsInfoHighResolution(WINDOW_HEIGHT, WINDOW_WIDTH, &graphInt.pictureParams, &pointsInfo);
            calculateMatrixOfPointsInfoArrays(WINDOW_HEIGHT, WINDOW_WIDTH, &graphInt.pictureParams, &pointsInfo);
            graphInt.wasPictureUpdate = false;
        }
        IF_ERR_RETURN(drawBasedOnPointsInfoMatrix(&graphInt, &pointsInfo));
        showGradient(graphInt.window);

        auto end = std::chrono::high_resolution_clock::now();
        float deltaTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end-lastTimeOfFpsUpdate).count();
        //LOG_DEBUG_VARS(deltaTime);
        if (deltaTime > FPS_UPDATE_INTERVAL) {
            // update fps not so frequently, so user can comprehend what's written
            lastTimeOfFpsUpdate = end;
            fps = (float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
        }
        showFpsText(&graphInt, fps);

        window.display();
    }

    return 0;
}
