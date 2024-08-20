#include <SFML/Graphics.hpp>
#include "window.hpp"
#include <filesystem>

static void modifyCurrentWorkingDirectory() {
    // For cross platform use
    while (!std::filesystem::exists("resources"))
    {
        std::filesystem::current_path(std::filesystem::current_path().parent_path());
    }
    auto cwd = std::filesystem::current_path();
}

Window::Window(const sf::String& title) 
: sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), title, sf::Style::Close) {

    modifyCurrentWorkingDirectory();

    fpsTextFont.loadFromFile("resources/FiraCode-Regular.ttf");
    fpsText.setFont(fpsTextFont);
    fpsText.setCharacterSize(20);
    fpsText.setOutlineColor(sf::Color::Black);
    fpsText.setOutlineThickness(0.5);
    fpsText.setFillColor(sf::Color::Green);

    this->setFramerateLimit(MAX_FRAME_RATE);
    this->setVerticalSyncEnabled(false);
}

float Window::calculateDelta() {

    if (refreshClock.getElapsedTime() >= FPS_REFRESH_RATE) {

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