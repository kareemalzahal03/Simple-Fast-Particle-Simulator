#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "config.hpp"

// An abstraction of the SFML window class, with added functionality for
// calculating, capping, and displaying FPS.
class Window : public sf::RenderWindow {
public:

    // Constructor
    Window(uint width, uint height, const sf::String title);

    // To be called every iteration, returns time difference in seconds
    // between time last executed and updates the FPS text.
    float calculateDelta();

    // Draw the FPS to the top left of the screen.
    void drawFPS();

private:

    sf::Clock fpsClock;
    sf::Clock refreshClock;
    sf::Text fpsText;
    sf::Font fpsTextFont;
    int frames = 0;
};