#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;


class FractalImage {
    int fractalSize = 1024;
    uint8_t *fractalArray;
    Image fractal;
    Texture fractalTexture;
    RectangleShape fractalDisplay;

public:
    FractalImage(int n);
    ~FractalImage();
    void update(int n);
    void draw(RenderWindow &window);
};
