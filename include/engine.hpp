#pragma once
#include <SFML/Graphics.hpp>
#include "window.hpp"
#include "particlesim.hpp"
#include "accsleep.hpp"

class Engine {
public:

    Engine();
    void run();

private:

    sf::Time frame();
    void onKeyPressed(sf::Keyboard::Key code);
    void interpretCommand();

    const int width = 1450;
    const int height = 900;
    const int targetfps = 90;

    Window window;
    ParticleSimulator particlesim;
    AccurateSleep accuratesleep;
    sf::Clock frameClock;
    std::string command;
};
