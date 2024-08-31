#pragma once
#include <SFML/Graphics.hpp>
#include "window.hpp"
#include "particle.hpp"
#include "particlemanager.hpp"
#include "parallelize.hpp"

class ParticleSimulator {
public:

    ParticleSimulator(int width, int height);

    void simStep(float delta);

    void onEvent(sf::Event& event, sf::Vector2u size);

    void drawContent(sf::RenderWindow& window);

    void setValue(std::string name, float value);

private:

    void calculateDensities();

    void calculateParticleForces();

    void moveParticles(float delta);

    void moveParticle(Particle& particle, float delta);

    ParticleManager pm;
    Parallelize parallelize;

    const int width;
    const int height;
    const int circleradius = 100;
    const int spawn = 2000;
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
