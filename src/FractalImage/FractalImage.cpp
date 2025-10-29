#include "FractalImage.h"
#include "NewtonFractal.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;


FractalImage::FractalImage(int n)
{
    //Allocating space for the fractal pixel array
    fractalArray = new uint8_t[fractalSize * fractalSize * 4];

    //Generating the first fractal
    update(n);

    //Setting the display
    fractalDisplay.setTexture(&fractalTexture);
    fractalDisplay.setSize(Vector2f(1024, 1024));
    fractalDisplay.setOrigin(Vector2f(0, 0));
    fractalDisplay.setPosition(Vector2f(0, 0));
}


FractalImage::~FractalImage() {
    delete [] fractalArray;
}


//Create a Newton's fractal with a new n
void FractalImage::update(int n) {
    ispc::computeNewton(n, fractalSize, fractalArray);
    //Finding a range of iterations
    uint8_t range[2] = {255, 0};
    for (int i = 0; i < fractalSize * fractalSize; i++) {
        if (fractalArray[i * 4 + 1] > range[1]) {
            range[1] = fractalArray[i * 4 + 1];
        }
        if (fractalArray[i * 4 + 1] < range[0]) {
            range[0] = fractalArray[i * 4 + 1];
        }
    }
    ispc::save_image(n, fractalSize, fractalArray, range);
    //Setting the texture
    fractal.resize(Vector2u(fractalSize, fractalSize), fractalArray);
    fractalTexture.loadFromImage(fractal);
}


//Drawing the fractal
void FractalImage::draw(RenderWindow &window) {
    window.draw(fractalDisplay);
}


