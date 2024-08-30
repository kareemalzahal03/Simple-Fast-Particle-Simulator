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

ParticleSimulator::ParticleSimulator(int width, int height):
    width(width), height(height), pm(width, height) {

    // Spawn particles
    for (int x = 0; x < spawn; x++) {
        sf::Vector2f randomPos(
            float(rand())/RAND_MAX*width,
            float(rand())/RAND_MAX*height);
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
    
    calculateDensities();

    calculateParticleForces();
    
    moveParticles(delta);

    pm.sortParticles();
}

void ParticleSimulator::calculateDensities() {

    parallelize.for_each(pm,[this](Particle& particle) {

        float density = 0.1;

        for (auto other = pm.begin(particle.position); other != pm.end(); ++other) {

            float dist = Magnitude(particle.position - other->position);

            density += SpikyKernelPow2(dist, smradius) * mass;
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
    
        for (auto other = pm.begin(particle.position); other != pm.end(); ++other) {

            sf::Vector2f dir = particle.position - other->position;

            // PRESSURE
            pressureForce -= Unit(dir) * SharedPressure(particle.density, other->density, targetdensity, pressure)
            * DerivativeSpikyPow2(Magnitude(dir),smradius) * mass / other->density;

            // VISCOSITY
            viscosityForce += (other->velocity - particle.velocity)
            * CosKernel(Magnitude(dir), smradius) * viscosity;
        }

        // GRAVITY
        gravityForce = sf::Vector2f(0, gravity);

        // MOUSE
        sf::Vector2f dirToMouse = circle.getPosition() - particle.position;
        if (isMouseHeld) mouseForce += 
            (isRepelling ? -1 : 1) 
            * CosKernel(Magnitude(dirToMouse), 2*circleradius) 
            * mouse * Unit(dirToMouse);

        // TOTAL FORCE
        particle.force = pressureForce + viscosityForce + gravityForce + mouseForce;
    });
}

void ParticleSimulator::moveParticles(float delta) {

    parallelize.for_each(pm,[delta,this](Particle& particle) {

        sf::Vector2f particle_acceleration = particle.force / particle.density;
        particle.velocity += particle_acceleration * delta;
        moveParticle(particle, delta);
    });
}

void ParticleSimulator::drawContent(sf::RenderWindow& window) {    
    
    if (debug) pm.drawBoxes(window);

    // Draw Particles
    sf::CircleShape particleShape(particleRaduis);
    particleShape.setOrigin(particleRaduis/2,particleRaduis/2);

    for (auto& particle : pm) {

        particleShape.setPosition(particle.position);
        particleShape.setFillColor(colorBlend(sf::Color::Cyan,sf::Color::Blue,
            (particle.density-targetdensity)/3));

        window.draw(particleShape);
    }

    if (debug && !isMouseHeld) 
    for (auto p = pm.begin(circle.getPosition()); p != pm.end(); ++p) {

        particleShape.setPosition(p->position);
        particleShape.setFillColor(sf::Color::Red);

        window.draw(particleShape);
    }

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
}

void ParticleSimulator::moveParticle(Particle& particle, float delta) {

    sf::Vector2f currPos = particle.position;
    sf::Vector2 moveVec = delta * particle.velocity;
    sf::Vector2f newPos = currPos + moveVec;

    while (newPos.x < 0 || newPos.x > width || newPos.y < 0 || newPos.y > height) {

        if (newPos.x < 0) {

            sf::Vector2f collisionPos(0,(-currPos.x)*moveVec.y/moveVec.x+currPos.y);

            if (0 <= collisionPos.y && collisionPos.y <= height) {

                currPos = collisionPos;
                moveVec = collision*(newPos-currPos);
                moveVec.x *= -1;
                particle.velocity.x *= - 1;
            }
        }
        
        if (newPos.x > width) {

            sf::Vector2f collisionPos(width,(width-currPos.x)*moveVec.y/moveVec.x+currPos.y);

            if (0 <= collisionPos.y && collisionPos.y <= height) {

                currPos = collisionPos;
                moveVec = collision*(newPos-currPos);
                moveVec.x *= -1;
                particle.velocity.x *= - 1;
            }
        }
        
        if (newPos.y < 0) {

            sf::Vector2f collisionPos((-currPos.y)*moveVec.x/moveVec.y+currPos.x,0);

            if (0 <= collisionPos.x && collisionPos.x <= width) {

                currPos = collisionPos;
                moveVec = collision*(newPos-currPos);
                moveVec.y *= -1;
                particle.velocity.y *= - 1;
            }
        }
        
        if (newPos.y > height) {

            sf::Vector2f collisionPos((height-currPos.y)*moveVec.x/moveVec.y+currPos.x,height);
            
            if (0 <= collisionPos.x && collisionPos.x <= width) {

                currPos = collisionPos;
                moveVec = collision*(newPos-currPos);
                moveVec.y *= -1;
                particle.velocity.y *= - 1;
            }
        }

        particle.velocity *= collision;
        newPos = currPos + moveVec;
    }

    particle.position = newPos;
}

void ParticleSimulator::setValue(std::string name, float value) {
    if (name == "smradius") {
        smradius = value;
        pm.updateSmRadius(value);
    } else if (name == "viscosity") {
        viscosity = value;
    } else if (name == "gravity") {
        gravity = value;
    } else if (name == "mass") {
        mass = value;
    } else if (name == "targetdensity") {
        targetdensity = value;
    } else if (name == "pressure") {
        pressure = value;
    } else if (name == "mouse") {
        mouse = value;
    } else if (name == "collision") {
        collision = value;
    } else {
        std::cerr << "Bad name!" << std::endl;
    }
}

void ParticleSimulator::onEvent(sf::Event& event, sf::Vector2u size) {

    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Tab) debug = !debug;
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

    if (event.type == sf::Event::MouseMoved) 
        circle.setPosition(event.mouseMove.x*width/size.x, event.mouseMove.y*height/size.y);
}

