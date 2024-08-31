#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include "window.hpp"
#include "particlesim.hpp"

class Engine {
public:

    Engine();
    void run();

private:

    inline void step();
    inline void frame();

    inline void onKeyPressed(sf::Keyboard::Key code);
    inline void executeCommand(std::string command);
    inline void accurateSleep(sf::Time time, sf::Clock& clock);
    inline void processEvents();

    const int width = 1450;
    const int height = 900;
    const int targetSPS = 90;
    const int targetFPS = 60;
    const std::string filename = ".config";

    Window window;
    ParticleSimulator particlesim;
    sf::Clock stepClock;
    sf::Clock frameClock;
    std::string command;
};
