#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

class Config {
public:

    // Variables subject to change
    int width() { return values[WIDTH]; }
    int height() { return values[HEIGHT]; }
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
        WIDTH,HEIGHT,SMRADIUS,VISCOSITY,GRAVITY,MASS,
        TARGETDENSITY,PRESSURE,MOUSE,COLLISION,varcount
    };

    const char* names[varcount] = {
        "width","height","smradius","viscosity","gravity",
        "mass","targetdensity","pressure","mouse","collision"
    };

    float values[varcount];
};
