#include "graphicalInterface.hpp"
#include "../calcPointsInfo/calcPointsInfo.hpp"

const long double           SPEED_MOVEMENT  = 0.1;
constexpr const long double INC_SCALE_KOEF  = 1.3;
static_assert(INC_SCALE_KOEF - 1 > 1e-6);
constexpr const long double DEC_SCALE_KOEF  = 1.f / INC_SCALE_KOEF;
static_assert(abs(INC_SCALE_KOEF * DEC_SCALE_KOEF - 1) < 1e-6);

Errors constructGraphicalInterface(
    GraphicalInterface* graphInt,
    sf::RenderWindow* window,
    size_t windowWidth,
    size_t windowHeight
) {
    IF_ARG_NULL_RETURN(graphInt);

    PictureParameters pictureParams = {
        .pictureCenterX         = 0,
        .pictureCenterY         = 0,
        .scaleFactor            = 1,
    };

    *graphInt = {
        .WINDOW_WIDTH           = windowWidth,
        .WINDOW_HEIGHT          = windowHeight,
        .window                 = window,
        .pictureParams          = pictureParams,
        .wasPictureUpdate       = true
    };

    return STATUS_OK;
}

Errors windowEventsLoop(
    GraphicalInterface* graphInt
) {
    IF_ARG_NULL_RETURN(graphInt);

    sf::Event event = {};
    while (graphInt->window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            graphInt->window->close();
        }
    }

    return STATUS_OK;
}

Errors pictureParametresUpdate(
    GraphicalInterface* graphInt
) {
    IF_ARG_NULL_RETURN(graphInt);

    #define IS_KEY_PRESSED(key) sf::Keyboard::isKeyPressed(sf::Keyboard::key)
    float moveSpeed = SPEED_MOVEMENT / graphInt->pictureParams.scaleFactor;
    // if (IS_KEY_PRESSED(Left)) {
    //     graphInt->pictureParams.pictureCenterX -= moveSpeed;
    //     graphInt->wasPictureUpdate = true;
    // }
    // if (IS_KEY_PRESSED(Right)) {
    //     graphInt->pictureParams.pictureCenterX += moveSpeed;
    //     graphInt->wasPictureUpdate = true;
    // }
    // if (IS_KEY_PRESSED(Down)) {
    //     graphInt->pictureParams.pictureCenterY += moveSpeed;
    //     graphInt->wasPictureUpdate = true;
    // }
    // if (IS_KEY_PRESSED(Up)) {
    //     graphInt->pictureParams.pictureCenterY -= moveSpeed;
    //     graphInt->wasPictureUpdate = true;
    // }
    // if (IS_KEY_PRESSED(Z)) {
    //     if (IS_KEY_PRESSED(LShift)) {
    //         graphInt->pictureParams.scaleFactor *= DEC_SCALE_KOEF;
    //     } else {
    //         graphInt->pictureParams.scaleFactor *= INC_SCALE_KOEF;
    //     }
    //     graphInt->wasPictureUpdate = true;
    // }

    #define CHANGE_PIC_PARAM(condition, whatToChange)           \
        do {                                                    \
            if (condition) {                                    \
                graphInt->pictureParams.  whatToChange;         \
                graphInt->wasPictureUpdate = true;              \
            }                                                   \
        } while (0)

    // ASK: how to do this properly?
    CHANGE_PIC_PARAM(IS_KEY_PRESSED(Left),    pictureCenterX -= moveSpeed);
    CHANGE_PIC_PARAM(IS_KEY_PRESSED(Right),   pictureCenterX += moveSpeed);
    CHANGE_PIC_PARAM(IS_KEY_PRESSED(Down),    pictureCenterY += moveSpeed);
    CHANGE_PIC_PARAM(IS_KEY_PRESSED(Up),      pictureCenterY -= moveSpeed);
    CHANGE_PIC_PARAM(IS_KEY_PRESSED(Z) &&
                     IS_KEY_PRESSED(LShift),  scaleFactor *= DEC_SCALE_KOEF);
    CHANGE_PIC_PARAM(IS_KEY_PRESSED(Z) &&
                     !IS_KEY_PRESSED(LShift), scaleFactor *= INC_SCALE_KOEF);

    return STATUS_OK;
}

bool isWindowOpen(
    GraphicalInterface* graphInt
) {
    assert(graphInt != NULL);

    return graphInt->window->isOpen();
}

Errors closeWindowIfQuitKeyPressed(
    GraphicalInterface* graphInt
) {
    IF_ARG_NULL_RETURN(graphInt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        graphInt->window->close();
    }

    return STATUS_OK;
}

Errors drawBasedOnPointsInfoMatrix(
    GraphicalInterface* graphInt,
    PointsInfo*         pointsInfo
) {
    IF_ARG_NULL_RETURN(graphInt);
    IF_ARG_NULL_RETURN(pointsInfo);
    IF_ARG_NULL_RETURN(pointsInfo->escTimesMatrix);
    IF_ARG_NULL_RETURN(pointsInfo->lastPointRadiusMatrix);

    size_t windowWidth  = graphInt->WINDOW_WIDTH;
    size_t windowHeight = graphInt->WINDOW_HEIGHT;

    sf::Image screenBuffer;
     screenBuffer.create(windowWidth, windowHeight);
    sf::Texture screenTexture;
    screenTexture.create(windowWidth, windowHeight);

    size_t arrInd = 0;
    int MAX_NUM_OF_ITERS = getSimulationMaxNumOfIters();
    for (int pixelRow = 0; pixelRow < windowHeight; ++pixelRow) {
        for (int pixelCol = 0; pixelCol < windowWidth; ++pixelCol, ++arrInd) {
            int numOfIters     = pointsInfo->escTimesMatrix       [arrInd];
            float  pointRadius = pointsInfo->lastPointRadiusMatrix[arrInd];
            sf::Color color = getPixelColorSimple(numOfIters, MAX_NUM_OF_ITERS);
            //sf::Color color = getPixelColorDoubleLogSmoothing(numOfIters, pointRadius);
            screenBuffer.setPixel(pixelCol, pixelRow, color);
        }
    }

    screenTexture.update(screenBuffer);
    graphInt->window->draw(sf::Sprite(screenTexture));

    return STATUS_OK;
}
