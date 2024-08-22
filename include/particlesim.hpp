#pragma once
#include <SFML/Graphics.hpp>
#include "window.hpp"
#include "particle.hpp"
#include "particlemanager.hpp"
#include "parallelize.hpp"

class ParticleSimulator {
public:

    ParticleSimulator(int width, int height, int smradius);

    void simStep(float delta);

    void onEvent(sf::Event& event);

    void drawContent(Window& window);

private:

    void calculateDensities();

    void calculateParticleForces();

    void moveParticles(float delta);
    void moveParticle(Particle& particle, float delta);

    ParticleManager pm;
    Parallelize parallelize;

    bool debug = false;
    const int spawn = 1500;
    const int circleradius = 100;
    const sf::Color defaultColor = sf::Color::Cyan;
    const sf::Color repelColor = sf::Color::Red;
    const sf::Color attractColor = sf::Color::Green;

    const int width;
    const int height;
    const int smradius;


    sf::CircleShape circle;
    bool isMouseHeld = false;
    bool isRepelling = true;
};
