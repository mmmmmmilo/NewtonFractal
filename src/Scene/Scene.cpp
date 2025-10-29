#include "Scene.h"
#include <SFML/Graphics.hpp>
#include "../SaveImage/SaveImage.h"
#include "../SetEquation/SetEquation.h"
#include "../FractalImage/FractalImage.h"

using namespace std;
using namespace sf;

Scene::Scene() :
    window(VideoMode({1024, 1024}), "Newton fractal"),
    set_equation(n),
    fractal_image(n)
{
    window.setFramerateLimit(60);
    //Connecting the triggers
    set_equation.triggerDraw = [this]() {draw();};
    set_equation.triggerFractalUpdate = [this] {fractal_image.update(n);};
    save_image.triggerSave = [this]() {
        draw();
        save_image.save(n);
    };
}


//Drawing all elements on the window
void Scene::draw() {
    window.clear(Color::Black);

    fractal_image.draw(window);
    set_equation.draw(window);
    save_image.draw(window);

    window.display();
}


//Updating all elements
void Scene::update() {
    set_equation.update(window, n);
    save_image.update(window);
}


//Basically a main loop
void Scene::loop() {
    //Handling closing the window + updating and drawing the objects
    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        update();
        draw();
    }
}




