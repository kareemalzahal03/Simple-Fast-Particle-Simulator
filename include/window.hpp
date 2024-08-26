#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

class Window : public sf::RenderWindow {
public:

    // Constructor
    Window(int width, int height, const sf::String title);

    // Draw the FPS, command to the top left of the screen.
    void drawText();

    void updateCommandText(std::string command);

    void updateFPSText(int targetFPS);

private:

    sf::Clock clock;
    int frames = 0;
    std::string command;
    sf::Text commandText;
    sf::Text fpsText;
    sf::Font font;
};