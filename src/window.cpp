#include <SFML/Graphics.hpp>
#include "window.hpp"
#include <filesystem>

Window::Window(const sf::String title) 
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

    this->setFramerateLimit(70);
}

void Window::onEvent(sf::Event& event) {
    Config& config = Config::get();

    if (event.type == sf::Event::Closed){

        close();

    } else if (event.type == sf::Event::KeyPressed){

        if (0 <= event.key.code && event.key.code <= 25)
            command += 'a'+event.key.code;

        if (26 <= event.key.code && event.key.code <= 35)
            command += '0'-26+event.key.code;

        if (event.key.code == sf::Keyboard::Period)
            command += '.';

        if (event.key.code == sf::Keyboard::Space)
            command += ' ';
        
        if (event.key.code == sf::Keyboard::Backspace) {
            if (!command.empty()) command.pop_back();
        }

        if (event.key.code == sf::Keyboard::Enter) {
            config(command);
            command.clear();
        }

        commandText.setString("> "+command+'_');
    }
}

void Window::drawFPS(float delta) {

    if (delta) fpsText.setString(
        std::to_string(static_cast<int>(1.f/delta)) + " fps");
        
    this->draw(fpsText);
    this->draw(commandText);
}