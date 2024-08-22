#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "config.hpp"
#include <string>

// An abstraction of the SFML window class, with added functionality for
// calculating, capping, and displaying FPS.
class Window : public sf::RenderWindow {
public:

    // Constructor
    Window(int width, int height, const sf::String title);

    void onEvent(sf::Event& event);

    // Draw the FPS, command to the top left of the screen.
    void drawFPS(float delta);

private:

    std::string command;
    sf::Text commandText;
    sf::Text fpsText;
    sf::Font font;
};