#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include "particlesim.hpp"
#include "window.hpp"
#include "particle.hpp"
#include "particlemanager.hpp"
#include <iostream>
#include "mathfunctions.hpp"
#include <stdlib.h>

ParticleSimulator::ParticleSimulator() {

    // Spawn particles
    for (int x = 0; x < particleSpawn; x++) {
        pm.addParticle(Particle());
    }

    // Init circle
    circle.setRadius(MOUSE_RADIUS);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(5);
    circle.setOutlineColor(isRepelling ? repelColor : attractColor);
    circle.setOrigin(sf::Vector2f(MOUSE_RADIUS,MOUSE_RADIUS));
}

void ParticleSimulator::simStep(float delta) {

    pm.sortParticles();
    
    calculateDensities();

    calculateParticleForces();

    moveParticles(delta);
}

void ParticleSimulator::calculateDensities() {

    parallelize.for_each(pm,[this](Particle& particle) {

        float density = 0.1;

        for (auto other = pm.begin(particle.getPosition()); other != pm.end(); ++other) {

            float dist = Magnitude(particle.getPosition()-other->getPosition());

            density += SpikyKernelPow2(dist) * mass;
        }

        particle.density = density;
    });
}

void ParticleSimulator::calculateParticleForces() {

    parallelize.for_each(pm,[this](Particle& particle) {

        sf::Vector2f pressureForce;
        sf::Vector2f viscosityForce;
        sf::Vector2f gravityForce;
        sf::Vector2f mouseForce;
    
        for (auto other = pm.begin(particle.getPosition()); other != pm.end(); ++other) {

            sf::Vector2f dir = particle.getPosition() - other->getPosition();

            // PRESSURE
            pressureForce -= Unit(dir) * SharedPressure(particle.density, other->density)
            * DerivativeSpikyPow2(Magnitude(dir)) * mass / other->density;

            // VISCOSITY
            viscosityForce += (other->velocity - particle.velocity)
            * CosKernel(Magnitude(dir), smoothingRadius) * VISCOSITY;
        }

        // GRAVITY
        gravityForce = sf::Vector2f(0, gravity);

        // MOUSE
        sf::Vector2f dirToMouse = circle.getPosition() - particle.getPosition();
        if (isMouseHeld) mouseForce += 
            (isRepelling ? -1 : 1) 
            * CosKernel(Magnitude(dirToMouse), 2*MOUSE_RADIUS) 
            * mouseForces * Unit(dirToMouse);

        // TOTAL FORCE
        particle.force = pressureForce + viscosityForce + gravityForce + mouseForce;
    });
}

void ParticleSimulator::moveParticles(float delta) {

    parallelize.for_each(pm,[delta](Particle& particle) {

        sf::Vector2f particle_acceleration = particle.force / particle.density;
        particle.velocity += particle_acceleration * delta;
        particle.moveParticle(delta);

        particle.setFillColor(colorBlend(sf::Color::Cyan,sf::Color::Blue,
            (particle.density-targetDensity)/3));
    });
}

void ParticleSimulator::drawContent(Window& window, bool debug) {

    // Color particles close to mouse
    if (debug and !isMouseHeld) 
    for (auto p = pm.begin(circle.getPosition()); p != pm.end(); ++p) {
        p->setFillColor(sf::Color::Red);
    }
    
    // Draw Particles
    for (auto& particle : pm) 
        window.draw(particle);

    // Draw Circle
    if (isMouseHeld) {

        window.draw(circle);

    } else if (debug) {

        sf::CircleShape smRadiusCircle = circle;
        smRadiusCircle.setRadius(smoothingRadius);
        smRadiusCircle.setOutlineColor(sf::Color::White);
        smRadiusCircle.setOrigin(smoothingRadius,smoothingRadius);
        window.draw(smRadiusCircle);
    }
        
    // Draw Boxes
    if (debug) {

        sf::RectangleShape r(sf::Vector2f(smoothingRadius,smoothingRadius));
        r.setFillColor(sf::Color(0,0,0,0));
        r.setOutlineColor(sf::Color::White);
        r.setOutlineThickness(0.5);

        for (int x = 0; x < WIDTH/smoothingRadius; ++x) {
            for (int y = 0; y < HEIGHT/smoothingRadius; ++y) {
                r.setPosition(sf::Vector2f(x*smoothingRadius, y*smoothingRadius));
                window.draw(r);
            }
        }
    }
}

void ParticleSimulator::onMouseEvent(sf::Event event) {

    if (event.type == sf::Event::MouseButtonPressed and 
        event.mouseButton.button == sf::Mouse::Left) { isMouseHeld = true; }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) { isMouseHeld = false; }
        else if (event.mouseButton.button == sf::Mouse::Right) { 
            isRepelling = !isRepelling; 
            circle.setOutlineColor(isRepelling ? repelColor : attractColor);
        }
    } 

    if (event.type == sf::Event::MouseMoved) 
        { circle.setPosition(event.mouseMove.x, event.mouseMove.y); }
}
