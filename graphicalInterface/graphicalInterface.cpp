#include "graphicalInterface.hpp"
//#include "mandelbrotColoring.cpp"

const float SPEED_MOVEMENT  = 0.1;
const float SCALING_SPEED   = 0.1;
const float INC_SCALE_KOEF  = 1 + SCALING_SPEED;
const float DEC_SCALE_KOEF  = 1 - SCALING_SPEED;

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

    float moveSpeed = SPEED_MOVEMENT / graphInt->pictureParams.scaleFactor;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        printf("scale factor : %f\n", graphInt->pictureParams.scaleFactor);
        //graphInt->pictureParams.pictureCenterX -= 1000 * (float)graphInt->pictureParams.scaleFactor / graphInt->WINDOW_WIDTH;
        graphInt->pictureParams.pictureCenterX -= moveSpeed;
        graphInt->wasPictureUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        //graphInt->pictureParams.pictureCenterX -= 1000 * (float)graphInt->pictureParams.scaleFactor / graphInt->WINDOW_WIDTH;
        graphInt->pictureParams.pictureCenterX += moveSpeed;
        graphInt->wasPictureUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        printf("scale factor : %f\n", graphInt->pictureParams.scaleFactor);
        //graphInt->pictureParams.pictureCenterX -= 1000 * (float)graphInt->pictureParams.scaleFactor / graphInt->WINDOW_WIDTH;
        graphInt->pictureParams.pictureCenterY += moveSpeed;
        graphInt->wasPictureUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        //graphInt->pictureParams.pictureCenterX -= 1000 * (float)graphInt->pictureParams.scaleFactor / graphInt->WINDOW_WIDTH;
        graphInt->pictureParams.pictureCenterY -= moveSpeed;
        graphInt->wasPictureUpdate = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            graphInt->pictureParams.scaleFactor *= DEC_SCALE_KOEF;
            graphInt->wasPictureUpdate = true;
        } else {
            graphInt->pictureParams.scaleFactor *= INC_SCALE_KOEF;
            graphInt->wasPictureUpdate = true;
        }
    }

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
    for (int pixelRow = 0; pixelRow < windowHeight; ++pixelRow) {
        for (int pixelCol = 0; pixelCol < windowWidth; ++pixelCol, ++arrInd) {
            size_t numOfIters  = pointsInfo->escTimesMatrix       [arrInd];
            float  pointRadius = pointsInfo->lastPointRadiusMatrix[arrInd];
            sf::Color color = getPixelColorBasedOnIterationAndPoint(numOfIters, pointRadius);
            screenBuffer.setPixel(pixelCol, pixelRow, color);
        }
    }

    screenTexture.update(screenBuffer);
    graphInt->window->draw(sf::Sprite(screenTexture));

    return STATUS_OK;
}
