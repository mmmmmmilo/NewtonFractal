#include "SaveImage.h"
#include "NewtonFractal.h"
#include <SFML/Graphics.hpp>
#include <functional>

using namespace std;
using namespace sf;


SaveImage::SaveImage()  {
    //Setting the colors
    image_color = Color(189, 147, 249);
    press_color = Color(255, 121, 198);

    //Creating an array for saving fractals in a higher resolution
    fractalArray = new uint8_t[fractalSize * fractalSize * 4];

    //Setting the button
    error_code ec;
    filesystem::path exePath = std::filesystem::read_symlink("/proc/self/exe", ec);
    string path;
    ec ? path = filesystem::current_path() : path = exePath.parent_path();
    texture.loadFromFile(path + "/Resources/Save.png");
    texture.setSmooth(true);

    save_rect.setTexture(&texture);
    save_rect.setSize(Vector2f(24, 24));
    save_rect.setOrigin(Vector2f(0, 0));
    save_rect.setPosition(Vector2f(996, 4));
    save_rect.setFillColor(image_color);
}


SaveImage::~SaveImage() {
    delete [] fractalArray;
}


//Enlarging the button and triggering the fractal save if needed
void SaveImage::hover_save(RenderWindow &window) {
    save_rect.setFillColor(image_color);
    Vector2i mouse_pos = Mouse::getPosition(window);
    bool is_hovering = false;

    //Checking if mouse is hovering over the download button
    if ((float)mouse_pos.x >= save_rect.getPosition().x && (float)mouse_pos.x <= save_rect.getPosition().x + save_rect.getSize().x &&
        (float)mouse_pos.y >= save_rect.getPosition().y && (float)mouse_pos.y <= save_rect.getPosition().y + save_rect.getSize().y) {
        is_hovering = true;
        }

    //Enlarging the download button
    if (is_hovering) {
        save_rect.setSize(Vector2f(26, 26));
        save_rect.setPosition(Vector2f(995, 3));

        //Triggering the save of a fractal on screen
        if (Mouse::isButtonPressed(Mouse::Button::Left) && was_LMB_pressed != Mouse::isButtonPressed(Mouse::Button::Left))
        {
            save_rect.setFillColor(press_color);
            triggerSave();
        }
    }
    else {
        save_rect.setSize(Vector2f(24, 24));
        save_rect.setPosition(Vector2f(996, 4));
    }
}


//Calling relevant methods every frame
void SaveImage::update(RenderWindow &window) {
    hover_save(window);
    was_LMB_pressed = Mouse::isButtonPressed(Mouse::Button::Left);
}


//Drawing the button
void SaveImage::draw(RenderWindow &window) {
    window.draw(save_rect);
}


//Generating the displayed fractal in a higher resolution and saving it
void SaveImage::save(int n) {
    ispc::computeNewton(n, fractalSize, fractalArray);
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
    fractal.resize(Vector2u(fractalSize, fractalSize), fractalArray);
    fractal.saveToFile("Newton_Fractal_" + to_string(n) + ".png");
}



