#include "graphicalInterface.hpp"
#include "../calcPointsInfo/calcPointsInfo.hpp"

// const long double           SPEED_MOVEMENT  = 0.02;
// constexpr const long double INC_SCALE_KOEF  = 1.05;

const char*                 PATH2FONT               = "assets/latoMediumFont.ttf";
const unsigned int          FONT_SIZE               = 20;
const size_t                MAX_FPS_STRING_LEN      = 15;

const long double           SPEED_MOVEMENT_WITH_FPS = (long double)1.0;
constexpr long double       SCALE_SPEED_WITH_FPS    = (long double)2.0;

// static_assert(INC_SCALE_KOEF - 1 > 1e-6);
// constexpr const long double DEC_SCALE_KOEF  = 1.f / INC_SCALE_KOEF;
// static_assert(abs(INC_SCALE_KOEF * DEC_SCALE_KOEF - 1) < 1e-6);

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
    GraphicalInterface* graphInt,
    const float fps
) {
    IF_ARG_NULL_RETURN(graphInt);

    #define IS_KEY_PRESSED(key) sf::Keyboard::isKeyPressed(sf::Keyboard::key)
    //float moveSpeed = SPEED_MOVEMENT / graphInt->pictureParams.scaleFactor;

    // move speed according to FPS
    float moveSpeed = SPEED_MOVEMENT_WITH_FPS / graphInt->pictureParams.scaleFactor;
    moveSpeed /= fps;

    bool flag = false;
    sf::Vector3f delta(0, 0, 1); // delta.x - x picture center coord,
                                 // delta.y - y picture center coord,
                                 // delta.z - scale koefficient

    if (IS_KEY_PRESSED(Left))                         delta.x = -1,                                 flag = true;
    if (IS_KEY_PRESSED(Right))                        delta.x =  1,                                 flag = true;
    if (IS_KEY_PRESSED(Up))                           delta.y = -1,                                 flag = true;
    if (IS_KEY_PRESSED(Down))                         delta.y =  1,                                 flag = true;
    if (IS_KEY_PRESSED(Z) &&  IS_KEY_PRESSED(LShift)) delta.z = (fps - SCALE_SPEED_WITH_FPS) / fps, flag = true;
    if (IS_KEY_PRESSED(Z) && !IS_KEY_PRESSED(LShift)) delta.z = (fps + SCALE_SPEED_WITH_FPS) / fps, flag = true;

    graphInt->wasPictureUpdate             |= flag;
    graphInt->pictureParams.pictureCenterX += delta.x * moveSpeed;
    graphInt->pictureParams.pictureCenterY += delta.y * moveSpeed;
    graphInt->pictureParams.scaleFactor    *= delta.z;

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

//     #define CHANGE_PIC_PARAM(condition, whatToChange)           \
//         do {                                                    \
//             if (condition) {                                    \
//                 graphInt->pictureParams.  whatToChange;         \
//                 graphInt->wasPictureUpdate = true;              \
//             }                                                   \
//         } while (0)
//
//     // ASK: how to do this properly?
//     CHANGE_PIC_PARAM(IS_KEY_PRESSED(Left),    pictureCenterX -= moveSpeed);
//     CHANGE_PIC_PARAM(IS_KEY_PRESSED(Right),   pictureCenterX += moveSpeed);
//     CHANGE_PIC_PARAM(IS_KEY_PRESSED(Down),    pictureCenterY += moveSpeed);
//     CHANGE_PIC_PARAM(IS_KEY_PRESSED(Up),      pictureCenterY -= moveSpeed);
//     CHANGE_PIC_PARAM(IS_KEY_PRESSED(Z) &&
//                      IS_KEY_PRESSED(LShift),  scaleFactor *= (fps - SCALE_SPEED_WITH_FPS) / fps);
//     CHANGE_PIC_PARAM(IS_KEY_PRESSED(Z) &&
//                      !IS_KEY_PRESSED(LShift), scaleFactor *= (fps + SCALE_SPEED_WITH_FPS) / fps);

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

    if (IS_KEY_PRESSED(Q) || IS_KEY_PRESSED(Escape)) {
        graphInt->window->close();
    }

    return STATUS_OK;
}

Errors showFpsText(
    GraphicalInterface* graphInt,
    float fps
) {
    IF_ARG_NULL_RETURN(graphInt);

    sf::Font font = {};
    IF_NOT_CONDITION_RETURN(font.loadFromFile(PATH2FONT), COULDNT_LOAD_FONT_ERROR);

    char stringBuff[MAX_FPS_STRING_LEN] = {};
    snprintf(stringBuff, MAX_FPS_STRING_LEN, "%.2f", fps);

    sf::Text text;
    text.setFont(font);
    text.setString(stringBuff);
    text.setCharacterSize(FONT_SIZE);
    graphInt->window->draw(text);

    return STATUS_OK;
}

Errors drawBasedOnPointsInfoMatrix(
    GraphicalInterface* graphInt,
    PointsInfo*         pointsInfo
) {
    //printf("hello\n");
    IF_ARG_NULL_RETURN(graphInt);
    IF_ARG_NULL_RETURN(pointsInfo);
    IF_ARG_NULL_RETURN(pointsInfo->escTimesMatrix);
    IF_ARG_NULL_RETURN(pointsInfo->lastPointRadiusMatrix);

    size_t windowWidth  = graphInt->WINDOW_WIDTH;
    size_t windowHeight = graphInt->WINDOW_HEIGHT;

//    printf("b");
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
            //printf("bib");
            sf::Color color = getPixelColorSimple(numOfIters, MAX_NUM_OF_ITERS);
            //sf::Color color = getPixelColorDoubleLogSmoothing(numOfIters, pointRadius);
            screenBuffer.setPixel(pixelCol, pixelRow, color);
        }
    }

    screenTexture.update(screenBuffer);
    graphInt->window->draw(sf::Sprite(screenTexture));

    return STATUS_OK;
}
