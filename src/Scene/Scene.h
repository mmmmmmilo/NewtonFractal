#pragma once
#include <SFML/Graphics.hpp>

#include "../SaveImage/SaveImage.h"
#include "../SetEquation/SetEquation.h"
#include "../FractalImage/FractalImage.h"

using namespace std;
using namespace sf;



class Scene {
    int n = 16;

    RenderWindow window;
    SetEquation set_equation;
    SaveImage save_image;
    FractalImage fractal_image;

public:
    Scene();
    void loop();

private:
    void draw();
    void update();
};

