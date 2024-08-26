#include <SFML/Graphics.hpp>
#include "window.hpp"
#include <filesystem>

Window::Window(int width, int height, const sf::String title) 
: sf::RenderWindow(sf::VideoMode(width, height), title) {
    
    // For cross platform use
    while (!std::filesystem::exists("resources"))
    std::filesystem::current_path(std::filesystem::current_path().parent_path());

    font.loadFromFile("resources/FiraCode-Regular.ttf");

    fpsText.setFont(font);
    fpsText.setCharacterSize(20);
    fpsText.setOutlineColor(sf::Color::Black);
    fpsText.setOutlineThickness(1);
    fpsText.setFillColor(sf::Color::White);

    commandText.setFont(font);
    commandText.setCharacterSize(20);
    commandText.setOutlineColor(sf::Color::Black);
    commandText.setOutlineThickness(1);
    commandText.setFillColor(sf::Color::White);
    commandText.setPosition(100,0);
    commandText.setString("> Type a command...");
}

void Window::drawText() {
    
    draw(fpsText);
    draw(commandText);
}

void Window::updateCommandText(std::string command) {

    commandText.setString("> "+command+'_');
}

void Window::updateFPSText(int targetFPS) {

    if (clock.getElapsedTime() >= sf::seconds(1)) {

        if (frames < targetFPS - 5)
            fpsText.setFillColor(sf::Color::Yellow);
        else
            fpsText.setFillColor(sf::Color::Green);

        fpsText.setString(std::to_string(frames)+" fps");
        frames = 0;
        clock.restart();
    }

    frames++;
}
