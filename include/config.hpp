#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

class Config {
public:

    // Variables subject to change
    float viscosity() { return values[VISCOSITY]; }
    float gravity() { return values[GRAVITY]; }
    float mass() { return values[MASS]; }
    float targetdensity() { return values[TARGETDENSITY]; }
    float pressure() { return values[PRESSURE]; }
    float mouse() { return values[MOUSE]; }
    float collision() { return values[COLLISION]; }

    void operator()(std::string command);
    static Config& get();

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
private:

    Config();
    void set(std::string name, float value);
    void printError();

    std::string filename = ".config";

    enum VAR {
        VISCOSITY,GRAVITY,MASS,TARGETDENSITY,PRESSURE,
        MOUSE,COLLISION,varcount
    };

    const char* names[varcount] = {
        "viscosity","gravity","mass","targetdensity",
        "pressure","mouse","collision"
    };

    float values[varcount];
};

// WINDOW

// const int WIDTH = 1450;
// const int HEIGHT = 900;
// const int MAX_FRAME_RATE = 60;

// // SIMULATOR

// const int particleSpawn = 1500;
// const float smRadius = 50;

// const int PARTICLE_RADIUS = 6;
// const int MOUSE_RADIUS = 100;

// COLORS

// const sf::Color defaultColor = sf::Color::Cyan;
// const sf::Color repelColor = sf::Color::Red;
// const sf::Color attractColor = sf::Color::Green;
