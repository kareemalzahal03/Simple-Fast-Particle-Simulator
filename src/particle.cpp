#include "particle.hpp"
#include "window.hpp"
#include <iostream>

Particle::Particle(sf::Vector2f pos) {

    this->setPosition(pos);
    this->setRadius(6);
    this->setOrigin(getRadius()/2,getRadius()/2);
}
