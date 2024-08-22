#include <SFML/Graphics.hpp>
#include "window.hpp"
#include <filesystem>

Window::Window(int width, int height, const sf::String title) 
: sf::RenderWindow(sf::VideoMode(width, height), title) {
    
    // For cross platform use
    while (!std::filesystem::exists("resources"))
    std::filesystem::current_path(std::filesystem::current_path().parent_path());

    fpsTextFont.loadFromFile("resources/FiraCode-Regular.ttf");
    fpsText.setFont(fpsTextFont);
    fpsText.setCharacterSize(20);
    fpsText.setOutlineColor(sf::Color::Black);
    fpsText.setOutlineThickness(0.5);
    fpsText.setFillColor(sf::Color::Green);

    this->setFramerateLimit(60);
    this->setVerticalSyncEnabled(false);
}

void Window::onEvent(sf::Event& event) {
    Config& config = Config::get();

    if (event.type == sf::Event::Closed){

        close();
        
    } else if (event.type == sf::Event::Resized) {

        config("width "+std::to_string(event.size.width));
        config("height "+std::to_string(event.size.height));
    }
}

// float Window::calculateDelta() {

//     if (refreshClock.getElapsedTime() >= sf::seconds(1)) {

//         fpsText.setString(std::to_string(static_cast<int>(
//         frames/refreshClock.restart().asSeconds())));
//         frames = 0;
//     }

//     frames++;

//     return fpsClock.restart().asSeconds();
// }

void Window::drawFPS(float delta) {

    if (delta) fpsText.setString(
        std::to_string(static_cast<int>(1.f/delta)) + " fps");
    this->draw(fpsText);
}