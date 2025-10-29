#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

using namespace std;
using namespace sf;

class SetEquation {
    Color background_color;
    Color text_color;
    Color variable_color;
    Color editing_color;

    RectangleShape background_left;
    RectangleShape background_bottom;
    CircleShape round_corner;
    Font font;
    Text equation_text;
    Text z_text;
    Text n_text;

public:
    SetEquation(int n);
    function<void()> triggerDraw;
    function<void()> triggerFractalUpdate;
    void update(RenderWindow &window, int &n);
    void draw(RenderWindow &window);

private:
    void hover_n(RenderWindow &window, int &n);
    void set_n(RenderWindow &window, int &n);
};

