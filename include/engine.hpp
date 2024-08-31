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

    Engine(int width, int height, const char* configfile);
    void run();

private:

    inline void step();
    inline void frame();

    inline void onKeyPressed(sf::Keyboard::Key code);
    inline void executeCommand(std::string command);
    inline void accurateSleep(sf::Time time, sf::Clock& clock);
    inline void processEvents();

    int targetSPS;
    int targetFPS;
    int spawn;
    
    Window window;
    ParticleSimulator particlesim;
    sf::Clock stepClock;
    sf::Clock frameClock;
    std::string command;
};
