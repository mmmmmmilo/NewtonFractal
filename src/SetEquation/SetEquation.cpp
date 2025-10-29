#include "SetEquation.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;


SetEquation::SetEquation(int n) :
    z_text(font),
    n_text(font),
    equation_text(font)
{
    //Setting colors
    background_color = Color(68, 71, 90);
    text_color = Color(248, 248, 242);
    variable_color = Color(189, 147, 249);
    editing_color = Color(139, 233, 253);

    //Setting pretty background
    Vector2f box_size = Vector2f(224, 32);
    float rounded_radius = 8;

    background_left.setSize(Vector2f(box_size.x, box_size.y - rounded_radius));
    background_left.setOrigin(Vector2f(0, 0));
    background_left.setPosition(Vector2f(1024 - background_left.getSize().x, 0));
    background_left.setFillColor(background_color);

    background_bottom.setSize(Vector2f(box_size.x - rounded_radius, box_size.y));
    background_bottom.setOrigin(Vector2f(0, 0));
    background_bottom.setPosition(Vector2f(1024 - background_bottom.getSize().x, 0));
    background_bottom.setFillColor(background_color);

    round_corner.setRadius(rounded_radius);
    round_corner.setOrigin(Vector2f(0, 0));
    round_corner.setPosition(Vector2f(1024 - box_size.x, box_size.y - rounded_radius * 2));
    round_corner.setFillColor(background_color);

    //Setting text
    error_code ec;
    filesystem::path exePath = std::filesystem::read_symlink("/proc/self/exe", ec);
    string path;
    ec ? path = filesystem::current_path() : path = exePath.parent_path();
    font.openFromFile(path + "/Resources/Mono.ttf");

    z_text.setFont(font);
    z_text.setCharacterSize(24);
    z_text.setFillColor(text_color);
    z_text.setOrigin(Vector2f(0, 0));
    z_text.setPosition(Vector2f(1024 - box_size.x + rounded_radius, 0));
    z_text.setString("z ");

    n_text.setFont(font);
    n_text.setCharacterSize(equation_text.getCharacterSize() * 2 / 3);
    n_text.setFillColor(variable_color);
    n_text.setOrigin(Vector2f(0, 0));
    n_text.setPosition(z_text.findCharacterPos(1));
    n_text.setString(to_string(n) + " ");

    equation_text.setFont(font);
    equation_text.setCharacterSize(24);
    equation_text.setFillColor(text_color);
    equation_text.setOrigin(Vector2f(0, 0));
    equation_text.setPosition(n_text.findCharacterPos(1));
    equation_text.setString(" - 1 = 0");
}


//Setting N and regenerating the fractal if needed
void SetEquation::set_n(RenderWindow &window, int &n) {
    n_text.setFillColor(editing_color);
    bool editing = true;
    bool changed = false;
    int previous_n = n;

    //Setting N and handling window closing outside the main loop
    while (editing) {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>()) {
                window.close();
                return;
            }
            if (event->is<Event::TextEntered>()) {
                auto text = event->getIf<sf::Event::TextEntered>();
                if (text->unicode == 13) {
                    editing = false;
                }
                else if (text->unicode == 8) {
                    n /= 10;
                    changed = true;
                }
                else if (text->unicode >= '0' && text->unicode <= '9') {
                    if (!changed) {
                        n = 0;
                        changed = true;
                    }
                    n = n * 10 + (int)text->unicode - '0';
                    if (n > 256) {
                        n = 256;
                    }
                }
            }
        }
        if (Mouse::isButtonPressed(Mouse::Button::Right))
            editing = false;
        if (!editing && n < 2) {
            n = 2;
        }

        //Preparing the new text and triggering the main draw method
        n_text.setString(to_string(n) + " ");
        equation_text.setPosition(n_text.findCharacterPos(to_string(n).size()));
        triggerDraw();
    }

    //Triggering fractal update if N changed
    n_text.setFillColor(variable_color);
    if (n != previous_n) {
        triggerFractalUpdate();
    }
}


//Enlarging N and calling set_n if needed
void SetEquation::hover_n(RenderWindow &window, int &n) {
    Vector2i mouse_pos = Mouse::getPosition(window);
    string n_string = n_text.getString();
    bool is_hovering = false;

    //Checking if mouse is hovering over N
    if ((float)mouse_pos.x >= n_text.getPosition().x && (float)mouse_pos.x <= n_text.findCharacterPos(n_string.size() - 1).x &&
        (float)mouse_pos.y >= n_text.getPosition().y && (float)mouse_pos.y <= n_text.getPosition().y + (float)n_text.getCharacterSize()) {
        is_hovering = true;
    }

    //Calling the setting of N if it was pressed
    if (is_hovering) {
        n_text.setStyle(Text::Bold);
        if (Mouse::isButtonPressed(Mouse::Button::Left))
        {
            set_n(window, n);
        }
    }
    else {
        n_text.setStyle(Text::Regular);
    }

    //Adjusting the equation position based on the size of N
    equation_text.setPosition(n_text.findCharacterPos(to_string(n).size()));
}


//Calling relevant methods every frame
void SetEquation::update(RenderWindow &window, int &n) {
    hover_n(window, n);
}


//Drawing the equation and background
void SetEquation::draw(RenderWindow &window) {
    //Drawing background
    window.draw(background_left);
    window.draw(background_bottom);
    window.draw(round_corner);

    //Drawing text
    window.draw(equation_text);
    window.draw(z_text);
    window.draw(n_text);
}
