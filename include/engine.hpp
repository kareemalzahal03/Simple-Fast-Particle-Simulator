#pragma once
#include <SFML/Graphics.hpp>
#include "window.hpp"
#include "particlesim.hpp"

class Engine {
public:

    Engine();
    void run();

private:

    sf::Time frame();
    void onKeyPressed(sf::Keyboard::Key code);
    void interpretCommand();
    void accurateSleep(sf::Time time);

    const int width = 1450;
    const int height = 900;
    const int targetfps = 90;

    Window window;
    ParticleSimulator particlesim;
    sf::Clock clock;
    std::string command;
};
