#include <stdio.h>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "errorsHandler/errorsHandler.hpp"
#include "graphicalInterface/graphicalInterface.hpp"
#include "calcPointsInfo/calcPointsInfo.hpp"

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 800;

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

    float fps = -1;
    while (isWindowOpen(&graphInt)) {
        IF_ERR_RETURN(windowEventsLoop(&graphInt));
        IF_ERR_RETURN(closeWindowIfQuitKeyPressed(&graphInt));

        auto start = std::chrono::high_resolution_clock::now();
        window.clear();
        IF_ERR_RETURN(pictureParametresUpdate(&graphInt));
        if (graphInt.wasPictureUpdate) {
            calculateMatrixOfPointsInfo(WINDOW_HEIGHT, WINDOW_WIDTH, &graphInt.pictureParams, &pointsInfo);
            graphInt.wasPictureUpdate = false;
        }
        IF_ERR_RETURN(drawBasedOnPointsInfoMatrix(&graphInt, &pointsInfo));
        //showGradient(graphInt.window);
        window.display();
        auto end   = std::chrono::high_resolution_clock::now();

        fps = (float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
        //printf("fps: %f\n", fps);
    }

    return 0;
}
