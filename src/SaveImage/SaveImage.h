#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

using namespace std;
using namespace sf;

class SaveImage {
    Color image_color;
    Color press_color;

    bool was_LMB_pressed = false;
    Texture texture;
    RectangleShape save_rect;

    int fractalSize = 4096;
    uint8_t *fractalArray;
    Image fractal;

public:
    SaveImage();
    ~SaveImage();
    function<void()> triggerSave;
    void update(RenderWindow &window);
    void draw(RenderWindow &window);
    void save(int n);

private:
    void hover_save(RenderWindow &window);
};
