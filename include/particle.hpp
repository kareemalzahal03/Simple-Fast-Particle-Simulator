#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

struct Particle {
    
    explicit Particle(sf::Vector2f position): position(position) {}

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f force;
    float density;
};
