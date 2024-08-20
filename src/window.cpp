#include <SFML/Graphics.hpp>
#include "window.hpp"

Window::Window(const sf::String& title) 
: sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), title, sf::Style::Close) {

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