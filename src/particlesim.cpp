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

ParticleSimulator::ParticleSimulator(int width, int height, int smradius):
    width(width),height(height),smradius(smradius),
    pm(width,height,smradius) {

    // Spawn particles
    for (int x = 0; x < spawn; x++) {
        // Set Random Position
        sf::Vector2f randomPos(
            static_cast <float> (rand ()) / static_cast <float> (RAND_MAX) * width, 
            static_cast <float> (rand ()) / static_cast <float> (RAND_MAX) * height);

        pm.addParticle(Particle(randomPos));
    }

    // Init circle
    circle.setRadius(circleradius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(5);
    circle.setOutlineColor(isRepelling ? repelColor : attractColor);
    circle.setOrigin(sf::Vector2f(circleradius,circleradius));
}

void ParticleSimulator::simStep(float delta) {

    pm.sortParticles();
    
    calculateDensities();

    calculateParticleForces();
    
    moveParticles(delta);
}

void ParticleSimulator::calculateDensities() {

    parallelize.for_each(pm,[this](Particle& particle) {

        Config& config = Config::get();

        float density = 0.1;

        for (auto other = pm.begin(particle.getPosition()); other != pm.end(); ++other) {

            float dist = Magnitude(particle.getPosition()-other->getPosition());

            density += SpikyKernelPow2(dist,smradius) * config.mass();
        }

        particle.density = density;
    });
}

void ParticleSimulator::calculateParticleForces() {

    parallelize.for_each(pm,[this](Particle& particle) {

        Config& config = Config::get();

        sf::Vector2f pressureForce;
        sf::Vector2f viscosityForce;
        sf::Vector2f gravityForce;
        sf::Vector2f mouseForce;
    
        for (auto other = pm.begin(particle.getPosition()); other != pm.end(); ++other) {

            sf::Vector2f dir = particle.getPosition() - other->getPosition();

            // PRESSURE
            pressureForce -= Unit(dir) * SharedPressure(particle.density, other->density)
            * DerivativeSpikyPow2(Magnitude(dir),smradius) * config.mass() / other->density;

            // VISCOSITY
            viscosityForce += (other->velocity - particle.velocity)
            * CosKernel(Magnitude(dir), smradius) * config.viscosity();
        }

        // GRAVITY
        gravityForce = sf::Vector2f(0, config.gravity());

        // MOUSE
        sf::Vector2f dirToMouse = circle.getPosition() - particle.getPosition();
        if (isMouseHeld) mouseForce += 
            (isRepelling ? -1 : 1) 
            * CosKernel(Magnitude(dirToMouse), 2*100) 
            * config.mouse() * Unit(dirToMouse);

        // TOTAL FORCE
        particle.force = pressureForce + viscosityForce + gravityForce + mouseForce;
    });
}

void ParticleSimulator::moveParticles(float delta) {

    parallelize.for_each(pm,[delta,this](Particle& particle) {

        sf::Vector2f particle_acceleration = particle.force / particle.density;
        particle.velocity += particle_acceleration * delta;
        moveParticle(particle, delta);

        Config& config = Config::get();
        particle.setFillColor(colorBlend(sf::Color::Cyan,sf::Color::Blue,
            (particle.density-config.targetdensity())/3));
    });
}

void ParticleSimulator::drawContent(Window& window) {

    // Color particles close to mouse
    if (debug && !isMouseHeld) 
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
        smRadiusCircle.setRadius(smradius);
        smRadiusCircle.setOutlineColor(sf::Color::White);
        smRadiusCircle.setOrigin(smradius,smradius);
        window.draw(smRadiusCircle);
    }
        
    // Draw Boxes
    if (debug) {

        sf::RectangleShape r(sf::Vector2f(smradius,smradius));
        r.setFillColor(sf::Color(0,0,0,0));
        r.setOutlineColor(sf::Color::White);
        r.setOutlineThickness(0.5);

        for (int x = 0; x < width/smradius; ++x) {
            for (int y = 0; y < height/smradius; ++y) {
                r.setPosition(sf::Vector2f(x*smradius, y*smradius));
                window.draw(r);
            }
        }
    }
}

void ParticleSimulator::moveParticle(Particle& particle, float delta) {
    Config& config = Config::get();

    sf::Vector2f currPos = particle.getPosition();
    sf::Vector2 moveVec = delta * particle.velocity;
    sf::Vector2f newPos = currPos + moveVec;

    while (newPos.x < 0 || newPos.x > width || newPos.y < 0 || newPos.y > height) {

        if (newPos.x < 0) {

            sf::Vector2f collisionPos(0,(-currPos.x)*moveVec.y/moveVec.x+currPos.y);

            if (0 <= collisionPos.y && collisionPos.y <= height) {

                currPos = collisionPos;
                moveVec = config.collision()*(newPos-currPos);
                moveVec.x *= -1;
                particle.velocity.x *= - 1;
            }
        }
        
        if (newPos.x > width) {

            sf::Vector2f collisionPos(width,(width-currPos.x)*moveVec.y/moveVec.x+currPos.y);

            if (0 <= collisionPos.y && collisionPos.y <= height) {

                currPos = collisionPos;
                moveVec = config.collision()*(newPos-currPos);
                moveVec.x *= -1;
                particle.velocity.x *= - 1;
            }
        }
        
        if (newPos.y < 0) {

            sf::Vector2f collisionPos((-currPos.y)*moveVec.x/moveVec.y+currPos.x,0);

            if (0 <= collisionPos.x && collisionPos.x <= width) {

                currPos = collisionPos;
                moveVec = config.collision()*(newPos-currPos);
                moveVec.y *= -1;
                particle.velocity.y *= - 1;
            }
        }
        
        if (newPos.y > height) {

            sf::Vector2f collisionPos((height-currPos.y)*moveVec.x/moveVec.y+currPos.x,height);
            
            if (0 <= collisionPos.x && collisionPos.x <= width) {

                currPos = collisionPos;
                moveVec = config.collision()*(newPos-currPos);
                moveVec.y *= -1;
                particle.velocity.y *= - 1;
            }
        }

        particle.velocity *= config.collision();
        newPos = currPos + moveVec;
    }

    particle.setPosition(newPos);
}

void ParticleSimulator::onEvent(sf::Event& event) {

    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::D) debug = !debug;
    }

    if (event.type == sf::Event::MouseButtonPressed && 
        event.mouseButton.button == sf::Mouse::Left) { isMouseHeld = true; }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) { isMouseHeld = false; }
        else if (event.mouseButton.button == sf::Mouse::Right) { 
            isRepelling = !isRepelling; 
            circle.setOutlineColor(isRepelling ? repelColor : attractColor);
        }
    } 

    if (event.type == sf::Event::MouseMoved) { 
        Config& config = Config::get();
        circle.setPosition(
            event.mouseMove.x*width/config.width(), 
            event.mouseMove.y*height/config.height());
    }
}
