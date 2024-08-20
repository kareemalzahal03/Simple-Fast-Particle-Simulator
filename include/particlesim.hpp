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

    void onMouseEvent(sf::Event event);

    void drawContent(Window& window, bool debug = false);

private:

    void calculateDensities();

    void calculateParticleForces();

    void moveParticles(float delta);

    ParticleManager pm;
    Parallelize parallelize;

    sf::CircleShape circle;
    bool isMouseHeld = false;
    bool isRepelling = true;
};
