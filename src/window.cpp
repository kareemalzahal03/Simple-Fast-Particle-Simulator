#include <SFML/Graphics.hpp>
#include "window.hpp"
#include <filesystem>

Window::Window(uint width, uint height, const sf::String title) 
: sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Close) {

    // For cross platform use
    while (!std::filesystem::exists("resources"))
    {
        std::filesystem::current_path(std::filesystem::current_path().parent_path());
    }
    auto cwd = std::filesystem::current_path();

    fpsTextFont.loadFromFile("resources/FiraCode-Regular.ttf");
    fpsText.setFont(fpsTextFont);
    fpsText.setCharacterSize(20);
    fpsText.setOutlineColor(sf::Color::Black);
    fpsText.setOutlineThickness(0.5);
    fpsText.setFillColor(sf::Color::Green);

    this->setFramerateLimit(60);
    this->setVerticalSyncEnabled(false);
}

float Window::calculateDelta() {

    if (refreshClock.getElapsedTime() >= sf::seconds(1)) {

        fpsText.setString(std::to_string(static_cast<int>(
        frames/refreshClock.restart().asSeconds())));
        frames = 0;
    }

    frames++;

    return fpsClock.restart().asSeconds();
}

void Window::drawFPS() {

    this->draw(fpsText);
}