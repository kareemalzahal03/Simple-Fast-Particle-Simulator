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

private:

    void calculateDensities();

    void calculateParticleForces();

    void moveParticles(float delta);

    void moveParticle(Particle& particle, float delta);

    ParticleManager pm;
    Parallelize parallelize;

    const int width;
    const int height;

    const int spawn = 2000;
    const int circleradius = 100;
    const int particleRaduis = 6;
    const sf::Color defaultColor = sf::Color::Cyan;
    const sf::Color repelColor = sf::Color::Red;
    const sf::Color attractColor = sf::Color::Green;

    bool debug = false;
    sf::CircleShape circle;
    bool isMouseHeld = false;
    bool isRepelling = true;
};
