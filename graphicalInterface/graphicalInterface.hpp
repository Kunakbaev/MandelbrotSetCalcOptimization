#ifndef GRAPHICAL_INTERFACE_GRAPHICAL_INTERFACE_HPP
#define GRAPHICAL_INTERFACE_GRAPHICAL_INTERFACE_HPP

#include <SFML/Graphics.hpp>
#include "../errorsHandler/errorsHandler.hpp"
#include "pictureParamsStruct.hpp"
#include "../calcPointsInfo/pointsInfoStruct.hpp"

struct GraphicalInterface {
    size_t                  WINDOW_WIDTH;        // should not be changed anywhere apart from constructor
    size_t                  WINDOW_HEIGHT;       // should not be changed anywhere apart from constructor

    sf::RenderWindow*       window;

    PictureParameters       pictureParams;
    bool                    wasPictureUpdate;
};

Errors constructGraphicalInterface(
    GraphicalInterface*     graphInt,
    sf::RenderWindow*       window,
    size_t                  windowWidth,
    size_t                  windowHeight
);

// loop through window's events (such as window move, close and etc.), without this loop window will just hang
Errors windowEventsLoop(
    GraphicalInterface* graphInt
);

// changes pictureCenterX, pictureCenterY and scale factor based on which key is pressedx
Errors pictureParametresUpdate(
    GraphicalInterface* graphInt,
    const float         fps
);

bool isWindowOpen(
    GraphicalInterface* graphInt
);

Errors closeWindowIfQuitKeyPressed(
    GraphicalInterface* graphInt
);

void precountGradientColorsArray();

void showGradient(sf::RenderWindow* window);

sf::Color getPixelColorDoubleLogSmoothing(
    int   numOfItersInteger,
    float pointRadius
);

sf::Color getPixelColorSimple(
    int   numOfItersInteger,
    int   maxNumOfIters
);

Errors showFpsText(
    GraphicalInterface* graphInt,
    float fps
);

Errors drawBasedOnPointsInfoMatrix(
    GraphicalInterface* graphInt,
    PointsInfo*         pointsInfo
);

#endif
