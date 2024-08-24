#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

// Program wide constants
inline const int width = 1450;
inline const int height = 900;
inline const int fps = 100;
inline const int spawn = 2000;
inline const int circleradius = 100;
inline const int particleRaduis = 6;
inline const sf::Color defaultColor = sf::Color::Cyan;
inline const sf::Color repelColor = sf::Color::Red;
inline const sf::Color attractColor = sf::Color::Green;

class Config {
public:

    // Variables subject to change
    float smradius() { return values[SMRADIUS]; }
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
        SMRADIUS,VISCOSITY,GRAVITY,MASS,TARGETDENSITY,
        PRESSURE,MOUSE,COLLISION,varcount
    };

    const char* names[varcount] = {
        "smradius","viscosity","gravity","mass",
        "targetdensity","pressure","mouse","collision"
    };

    float values[varcount];
};
