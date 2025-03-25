#include <cmath>

#include "graphicalInterface.hpp"

static const size_t GRADIENT_SCALE                 = 256;
static const size_t GRADIENT_COLORS_ARRAY_LEN      = 2048;
static const float  LOG2f                          = logf(2.0); // logf(2)

static const float colorGradientPositions[] = {
    0.0f,
    0.16f,
    0.42f,
    0.6425f,
    0.8575f,
    1.f
};

static const size_t NUM_OF_COLOR_POSITIONS = sizeof(colorGradientPositions) /
                                             sizeof(*colorGradientPositions);

static const float gradientColorValues[][3] = {
    {  0.f,   7.f, 100.f},
    { 32.f, 107.f, 203.f},
    {237.f, 255.f, 255.f},
    {255.f, 170.f,   0.f},
    {  0.f,   2.f,   0.f},
    {  0.f,   0.f,   0.f}
};

static sf::Color gradientColorsArray[GRADIENT_COLORS_ARRAY_LEN] = {};

static float linearInterpolation(float mi, float ma, float x) {
    return mi + x * (ma - mi);
}

void precountGradientColorsArray() {
    for (size_t i = 0; i < GRADIENT_COLORS_ARRAY_LEN; ++i) {
        float x = (float)i / GRADIENT_COLORS_ARRAY_LEN;
        size_t colorInd = 0;
        for (; colorInd + 1 < NUM_OF_COLOR_POSITIONS &&
               colorGradientPositions[colorInd + 1] < x; ++colorInd);

        float xOffset      = x - colorGradientPositions[colorInd];
        float colorSegmLen = colorGradientPositions[colorInd + 1] -
                             colorGradientPositions[colorInd];
        x = xOffset / colorSegmLen;

        uint8_t red   = (uint8_t)linearInterpolation(gradientColorValues[colorInd    ][0],
                                                     gradientColorValues[colorInd + 1][0], x);
        uint8_t green = (uint8_t)linearInterpolation(gradientColorValues[colorInd    ][1],
                                                     gradientColorValues[colorInd + 1][1], x);
        uint8_t blue  = (uint8_t)linearInterpolation(gradientColorValues[colorInd    ][2],
                                                     gradientColorValues[colorInd + 1][2], x);
        gradientColorsArray[i] = sf::Color(red, green, blue);
    }
}

void showGradient(sf::RenderWindow* window) {
    const uint32_t GRADIENT_WIDTH  = 800;
    const uint32_t GRADIENT_HEIGHT = 200;
    sf::Image screenBuffer;
    screenBuffer.create(GRADIENT_WIDTH, GRADIENT_HEIGHT);
    sf::Texture screenTexture;
    screenTexture.create(GRADIENT_WIDTH, GRADIENT_HEIGHT);

    for (uint32_t j = 0; j < (uint32_t)GRADIENT_WIDTH; ++j) {
        uint32_t gradientColInd = (j * (uint32_t)GRADIENT_COLORS_ARRAY_LEN) / GRADIENT_WIDTH;
        //assert(gradientColInd < GRADIENT_COLORS_ARRAY_LEN);
        sf::Color color = gradientColorsArray[gradientColInd];
        for (uint32_t i = 0; i < GRADIENT_HEIGHT; ++i)
            screenBuffer.setPixel(j, i, color);
    }

    screenTexture.update(screenBuffer);
    window->draw(sf::Sprite(screenTexture));
}

sf::Color getPixelColorSimple(
    int   numOfItersInteger,
    int   maxNumOfIters
) {
    size_t colorInd = ((float)numOfItersInteger / maxNumOfIters) * GRADIENT_COLORS_ARRAY_LEN;
    if (maxNumOfIters == numOfItersInteger)
        colorInd = GRADIENT_COLORS_ARRAY_LEN - 1;

    sf::Color color = gradientColorsArray[colorInd];
    return color;
}

// https://blogen.pasithee.fr/2019/01/06/smooth-coloring-of-mandelbrot/
// https://gist.github.com/Jofairden/56507b3184c7e16635ba66cd08c3d215
// https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia
sf::Color getPixelColorDoubleLogSmoothing(
    int   numOfItersInteger,
    float pointRadius
) {
    // from wikipedia
    float log_zn = logf(pointRadius) / 2;
    float nu     = logf(log_zn / LOG2f) / LOG2f;
    float smooth = (float)numOfItersInteger + 1 - nu;

    double smoothed = 1 - pow(0.99, smooth);
    size_t colorInd = (size_t)(smoothed * GRADIENT_COLORS_ARRAY_LEN);
    if (colorInd >= GRADIENT_COLORS_ARRAY_LEN)
        colorInd = GRADIENT_COLORS_ARRAY_LEN - 1;

    sf::Color color = gradientColorsArray[colorInd];
    return color;
}
