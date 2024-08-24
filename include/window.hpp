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
    Window(const sf::String title);

    void onEvent(sf::Event& event);

    // Draw the FPS, command to the top left of the screen.
    void drawFPS();

    void updateFPS(sf::Time frameTime);

private:

    // sf::Clock timeElapsed;
    // int frames = 0;
    std::string command;
    sf::Text commandText;
    sf::Text fpsText;
    sf::Font font;
};