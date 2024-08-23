#pragma once
#include <SFML/Graphics.hpp>
#include "config.hpp"
#include <cmath>

struct Particle {

    Particle(): position(
        float(rand())/RAND_MAX*width,
        float(rand())/RAND_MAX*height) {}
    
    Particle(sf::Vector2f position): position(position) {}

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f force;
    float density;
};
