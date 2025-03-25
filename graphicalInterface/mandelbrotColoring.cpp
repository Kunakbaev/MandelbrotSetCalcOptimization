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

// https://blogen.pasithee.fr/2019/01/06/smooth-coloring-of-mandelbrot/
// https://gist.github.com/Jofairden/56507b3184c7e16635ba66cd08c3d215
// https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia
sf::Color getPixelColorBasedOnIterationAndPoint(
    size_t numOfItersInteger,
    float  pointRadius
) {
    // from wikipedia
    float log_zn = logf(pointRadius) / 2;
    float nu     = logf(log_zn / LOG2f) / LOG2f;
    float smooth = (float)numOfItersInteger + 1 - nu;
    //smooth = numOfItersInteger;

    //double smoothed = log2(log2(pointRadius) / 2);  // log_2(log_2(pointRadius))
    double smoothed = 1 - pow(0.99, smooth);
    //double smoothed = log((float)(numOfIters + 1)) / log(MAX_NUM_OF_POINT_ITERATIONS);

    //int colorInd = (smoothed * GRADIENT_COLORS_ARRAY_LEN);
    //int colorInd = (int)(sqrt(numOfIters + 10 - smoothed) * GRADIENT_SCALE) % GRADIENT_COLORS_ARRAY_LEN;
    // printf("colorInd : %d\n", colorInd);
    // if (numOfIters == MAX_NUM_OF_POINT_ITERATIONS)
    //     colorInd = GRADIENT_COLORS_ARRAY_LEN - 1;

    // if (numOfIters >= 250) {
    //     printf("iters : %D, colorInd : %d, smoothed: %Lg\n", numOfIters, colorInd, smoothed);
    // }
    // colorInd = GRADIENT_COLORS_ARRAY_LEN - colorInd;
    //printf(""
    //int colorInd = ((float)numOfIters / MAX_NUM_OF_POINT_ITERATIONS) * GRADIENT_COLORS_ARRAY_LEN;
    size_t colorInd = (size_t)(smoothed * GRADIENT_COLORS_ARRAY_LEN);
    if (colorInd >= GRADIENT_COLORS_ARRAY_LEN)
        colorInd = GRADIENT_COLORS_ARRAY_LEN - 1;
    // if (numOfIters >= 250) {
    //     printf("colorInd : %d\n", colorInd);
    // }

//     int colorInd = 2047;
//     if (pointRadius > 1) {
        // float log_zn = logf(pointRadius) / 2;
        // float nu = logf(log_zn / logf(2)) / logf(2);
        // colorInd = numOfIters + 1 - nu;
//         if (colorInd > 270) printf("colorInd : %d\n", colorInd);
//         colorInd = ((float)colorInd / 270) * GRADIENT_COLORS_ARRAY_LEN;
//
//         // printf("pointRadius : %f\n", pointRadius);
//         // printf("nu : %f, logzn : %f, colorInd : %d\n", nu, log_zn, colorInd);
//     }

    sf::Color color = gradientColorsArray[colorInd];
    return color;
}
