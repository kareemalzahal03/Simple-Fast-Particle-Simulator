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

    sf::Time frame();
    inline void onKeyPressed(sf::Keyboard::Key code);
    inline void executeCommand(std::string command);
    inline void accurateSleep(sf::Time time);
    inline void processEvents();
    inline void displayContent();

    const int width = 1450;
    const int height = 900;
    const int targetfps = 90;
    const std::string filename = ".config";

    Window window;
    ParticleSimulator particlesim;
    sf::Clock clock;
    std::string command;
};
