#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Particle : public sf::CircleShape {
public:

    sf::Vector2f force;
    sf::Vector2f velocity;
    float density;

    Particle();
    void moveParticle(float delta);
};
