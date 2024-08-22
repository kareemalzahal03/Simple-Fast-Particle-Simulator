#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "config.hpp"

// An abstraction of the SFML window class, with added functionality for
// calculating, capping, and displaying FPS.
class Window : public sf::RenderWindow {
public:

    // Constructor
    Window(int width, int height, const sf::String title);

    void onEvent(sf::Event& event);

    // Draw the FPS to the top left of the screen.
    void drawFPS(float delta);

private:

    sf::Text fpsText;
    sf::Font fpsTextFont;
};