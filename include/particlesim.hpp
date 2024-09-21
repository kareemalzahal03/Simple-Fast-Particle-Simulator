#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "window.hpp"
#include "particlemanager.hpp"
#include "parallelize.hpp"
#include "mathfunctions.hpp"

class ParticleSimulator {
public:

    ParticleSimulator(int width, int height);

    void simStep(float delta);

    void onEvent(sf::Event& event, sf::Vector2u size);

    void drawContent(sf::RenderWindow& window);

    void setValue(std::string name, float value);

    void addParticles(int count);

private:

    void calculateDensities();

    void calculateParticleForces();

    ParticleManager pm;

    const int width;
    const int height;
    const int circleradius = 100;
    const int particleRaduis = 6;
    const sf::Color defaultColor = sf::Color::Cyan;
    const sf::Color repelColor = sf::Color::Red;
    const sf::Color attractColor = sf::Color::Green;

    float smradius;
    float viscosity;
    float gravity;
    float mass;
    float targetdensity;
    float pressure;
    float mouse;
    float collision;

    bool debug = false;
    sf::CircleShape circle;
    bool isMouseHeld = false;
    bool isRepelling = true;
};
