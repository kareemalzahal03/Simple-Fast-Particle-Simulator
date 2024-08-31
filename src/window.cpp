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

    spsText.setFont(font);
    spsText.setCharacterSize(20);
    spsText.setOutlineColor(sf::Color::Black);
    spsText.setOutlineThickness(1);
    spsText.setFillColor(sf::Color::White);
    spsText.setPosition(100,0);

    commandText.setFont(font);
    commandText.setCharacterSize(20);
    commandText.setOutlineColor(sf::Color::Black);
    commandText.setOutlineThickness(1);
    commandText.setFillColor(sf::Color::White);
    commandText.setPosition(200,0);
    commandText.setString("> Type a command...");
}

void Window::drawText(int targetFPS, int targetSPS) {
    
    if (clock.getElapsedTime() >= sf::seconds(1)) {

        if (frames < targetFPS / 2) 
            fpsText.setFillColor(sf::Color::Red);
        else if (frames < targetFPS * 9 / 10)
            fpsText.setFillColor(sf::Color::Yellow);
        else
            fpsText.setFillColor(sf::Color::Green);

        fpsText.setString(std::to_string(frames)+" fps");
        frames = 0;

        if (steps < targetSPS / 2) 
            spsText.setFillColor(sf::Color::Red);
        else if (steps < targetSPS * 9 / 10)
            spsText.setFillColor(sf::Color::Yellow);
        else
            spsText.setFillColor(sf::Color::Green);

        spsText.setString(std::to_string(steps)+" sps");
        steps = 0;

        clock.restart();
    }

    draw(spsText);
    draw(fpsText);
    draw(commandText);
}

void Window::updateCommandText(std::string command) {

    commandText.setString("> "+command+'_');
}

void Window::updateFPSText() { frames++; }

void Window::updateSPSText() { steps++; }
