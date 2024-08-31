#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

class Window : public sf::RenderWindow {
public:

    // Constructor
    Window(int width, int height, const sf::String title);

    // Draw the FPS, command to the top left of the screen.
    void drawText(int targetFPS, int targetSPS);

    void updateCommandText(std::string command);

    void updateFPSText();

    void updateSPSText();

private:

    sf::Clock clock;
    int frames = 0;
    int steps = 0;
    std::string command;
    sf::Text commandText;
    sf::Text fpsText;
    sf::Text spsText;
    sf::Font font;
};