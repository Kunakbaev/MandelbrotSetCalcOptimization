#include <stdio.h>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

const float  MAX_POINT_COORD                = powf(2, 10);
const size_t MAX_NUM_OF_POINT_ITERATIONS    = 256;
const float  EPS                            = 1e-3; // TODO:
const float  MAX_POINT_RADIUS_SQ            = powf(10.f, 2);
const int    WIDTH                          = 1000;
const int    HEIGHT                         = 1000;
const size_t GRADIENT_SCALE                 = 256;
const size_t GRADIENT_COLORS_ARRAY_LEN      = 2048;

const float colorGradientPositions[] = {
    0.0,
    0.16,
    0.42,
    0.6425,
    0.8575,
    1.0
};

const size_t NUM_OF_COLOR_POSITIONS = sizeof(colorGradientPositions) /
                                      sizeof(*colorGradientPositions);

const float gradientColorValues[][3] = {
    {  0,   7, 100},
    { 32, 107, 203},
    {237, 255, 255},
    {255, 170,   0},
    {  0,   2,   0},
    //{0, 8, 100}
};

size_t** matrixOfPointExitTime = NULL;

sf::Color gradientColorsArray[GRADIENT_COLORS_ARRAY_LEN] = {};

float linearInterpolation(float mi, float ma, float x) {
    return mi + x * (ma - mi);
}

void precountGradientColorsArray() {
    for (size_t i = 0; i < GRADIENT_COLORS_ARRAY_LEN; ++i) {
        float x = (float)i / GRADIENT_COLORS_ARRAY_LEN;
        int colorInd = 0;
        for (; colorInd + 1 < NUM_OF_COLOR_POSITIONS &&
               colorGradientPositions[colorInd + 1] < x; ++colorInd);

        float xOffset      = x - colorGradientPositions[colorInd];
        float colorSegmLen = colorGradientPositions[colorInd + 1] -
                             colorGradientPositions[colorInd];
        x = xOffset / colorSegmLen;

        uint8_t red   = (uint8_t)linearInterpolation(gradientColorValues[colorInd    ][0],
                                                   gradientColorValues[colorInd + 1][0], x);
        uint8_t green = roundf(linearInterpolation(gradientColorValues[colorInd    ][1],
                                                   gradientColorValues[colorInd + 1][1], x));
        uint8_t blue  = roundf(linearInterpolation(gradientColorValues[colorInd    ][2],
                                                   gradientColorValues[colorInd + 1][2], x));
        //printf("x : %f, i : %d, colorInd : %d, red : %f, bruh : %d\n", x, i, colorInd, red, (uint8_t)red);
        printf("colorInd : %d, %d %d %d\n", colorInd, red, green, blue);
        gradientColorsArray[i] = sf::Color((uint8_t)red, green, blue);
    }
}

void showGradient(sf::RenderWindow* window) {
    const size_t GRADIENT_WIDTH  = 800;
    const size_t GRADIENT_HEIGHT = 200;
    sf::Image screenBuffer;
    screenBuffer.create(GRADIENT_WIDTH, GRADIENT_HEIGHT);
    sf::Texture screenText;
    screenText.create(GRADIENT_WIDTH, GRADIENT_HEIGHT);

    for (int j = 0; j < GRADIENT_WIDTH; ++j) {
        int gradientColInd = roundf((float)j / GRADIENT_WIDTH * GRADIENT_COLORS_ARRAY_LEN);
        sf::Color color = gradientColorsArray[gradientColInd];
        for (int i = 0; i < GRADIENT_HEIGHT; ++i)
            screenBuffer.setPixel(j, i, color);
    }

    screenText.update(screenBuffer);
    //window->clear();
    window->draw(sf::Sprite(screenText));
    //window->display();
}

// https://gist.github.com/Jofairden/56507b3184c7e16635ba66cd08c3d215
// https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia
sf::Color getPixelColorBasedOnIterationAndPoint(
    size_t numOfItersInteger,
    float  pointRadius
) {
    // from wikipedia
    float log_zn = logf(pointRadius) / 2;
    float nu = logf(log_zn / logf(2)) / logf(2);
    float smooth = numOfItersInteger + 1 - nu;
    //int smooth = numOfIters;

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
    int colorInd = smoothed * GRADIENT_COLORS_ARRAY_LEN;
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

void calculateMatrixOfExitTimes(sf::RenderWindow* window) {
    sf::Image screenBuffer;
    screenBuffer.create(WIDTH, HEIGHT);
    sf::Texture screenText;
    screenText.create(WIDTH, HEIGHT);

    for (int pixelRow = 0; pixelRow < HEIGHT; ++pixelRow) {
        for (int pixelCol = 0; pixelCol < WIDTH; ++pixelCol) {
            float startX = ((float)pixelCol - WIDTH / 2 - 200) / WIDTH  * 2;
            float startY = ((float)pixelRow - HEIGHT / 2) / HEIGHT * 2;
            float curPointX = startX;
            float curPointY = startY;

            size_t numOfIters = 0;
            float pointRadius = 0;
            for (; numOfIters < MAX_NUM_OF_POINT_ITERATIONS; ++numOfIters) {
                if (abs(curPointX) > MAX_POINT_COORD ||
                    abs(curPointY) > MAX_POINT_COORD) break;
                float curXsq = curPointX * curPointX;
                float curYsq = curPointY * curPointY;
                float curXY  = curPointX * curPointY;

                pointRadius = curXsq + curYsq;
                if (pointRadius >= MAX_POINT_RADIUS_SQ) {
                    break;
                }

                curPointX = curXsq - curYsq + startX;
                curPointY = 2 * curXY       + startY;
            }

            // uint8_t tmp = sqrtf(sqrtf((float)numOfIters / MAX_NUM_OF_POINT_ITERATIONS)) * 255.f;
            // uint8_t red = 255 - tmp;
            // uint8_t green = (tmp & 1) * 64;
            // uint8_t blue = tmp;

            //sf::Color color(red, green, blue);
            sf::Color color = getPixelColorBasedOnIterationAndPoint(numOfIters, pointRadius);
            screenBuffer.setPixel(pixelCol, pixelRow, color);
        }
    }

    screenText.update(screenBuffer);
    //window->clear();
    window->draw(sf::Sprite(screenText));
    //window->display();
}

int main() {
    printf("Hello world!\n");

    precountGradientColorsArray();

    matrixOfPointExitTime = (size_t**)calloc(HEIGHT, sizeof(size_t*));
    for (int i = 0; i < HEIGHT; ++i) {
        matrixOfPointExitTime[i] = (size_t*)calloc(WIDTH, sizeof(size_t));
    }

    sf::VideoMode mode(WIDTH, HEIGHT);
    sf::RenderWindow window(mode, "i am window");
    float fps = -1;

    while (window.isOpen()) {
        sf::Event event = {};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            printf("Window exit\n");
            window.close();
            //break;
        }


        auto start = std::chrono::high_resolution_clock::now();
        window.clear();
        calculateMatrixOfExitTimes(&window);
        showGradient(&window);
        window.display();
        auto end   = std::chrono::high_resolution_clock::now();

        fps = (float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
        printf("fps: %f\n", fps);
    }

    return 0;
}
