#pragma once
#include <SFML/Graphics.hpp>
#include "window.hpp"
#include "particle.hpp"
#include "particlemanager.hpp"
#include "parallelize.hpp"

class ParticleSimulator {
public:

    ParticleSimulator();

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

    bool debug = false;
    sf::CircleShape circle;
    bool isMouseHeld = false;
    bool isRepelling = true;
};
