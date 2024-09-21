#include "particlesim.hpp"

ParticleSimulator::ParticleSimulator(int width, int height):
    width(width), height(height), pm(width, height) {    

    // Init circle
    circle.setRadius(circleradius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(5);
    circle.setOutlineColor(isRepelling ? repelColor : attractColor);
    circle.setOrigin(sf::Vector2f(circleradius,circleradius));
}

void ParticleSimulator::addParticles(int count) {

    for (int x = 0; x < count; x++) {
        sf::Vector2f randomPos(
            float(rand())/RAND_MAX*width,
            float(rand())/RAND_MAX*height);
        pm.addParticle(randomPos);
    }
}

void ParticleSimulator::simStep(float delta) {
    
    calculateDensities();

    calculateParticleForces();

    for (Particle& particle : pm) {
        sf::Vector2f particle_acceleration = particle.force / particle.density;
        particle.velocity += particle_acceleration * delta;
    }

    pm.moveParticles(delta,collision);
}

void ParticleSimulator::calculateDensities() {

    pm.parallel_for_each([this](Particle& particle) {

        float density = 0.1;

        for (const Particle& other : pm.neighbors(particle)) {

            float dist = Magnitude(particle.getPosition() - other.getPosition());

            density += SpikyKernelPow2(dist, smradius) * mass;
        }

        particle.density = density;
    });
}

void ParticleSimulator::calculateParticleForces() {

    pm.parallel_for_each([this](Particle& particle) {

        sf::Vector2f pressureForce;
        sf::Vector2f viscosityForce;
        sf::Vector2f gravityForce;
        sf::Vector2f mouseForce;
    
        // for (auto other = pm.begin(particle.position); other != pm.end(); ++other) {
        for (const Particle& other : pm.neighbors(particle)) {

            sf::Vector2f dir = particle.getPosition() - other.getPosition();

            // PRESSURE
            pressureForce -= Unit(dir) * SharedPressure(particle.density, other.density, targetdensity, pressure)
            * DerivativeSpikyPow2(Magnitude(dir),smradius) * mass / other.density;

            // VISCOSITY
            viscosityForce += (other.velocity - particle.velocity)
            * CosKernel(Magnitude(dir), smradius) * viscosity;
        }

        // GRAVITY
        gravityForce = sf::Vector2f(0, gravity);

        // MOUSE
        sf::Vector2f dirToMouse = circle.getPosition() - particle.getPosition();
        if (isMouseHeld) mouseForce += 
            (isRepelling ? -1 : 1) 
            * CosKernel(Magnitude(dirToMouse), 2*circleradius) 
            * mouse * Unit(dirToMouse);

        // TOTAL FORCE
        particle.force = pressureForce + viscosityForce + gravityForce + mouseForce;
    });
}

// void ParticleSimulator::moveParticles(float delta) {

//     pm.parallel_for_each([this,delta](Particle& particle) {

//         sf::Vector2f particle_acceleration = particle.force / particle.density;
//         particle.velocity += particle_acceleration * delta;
//         moveParticle(particle, delta);
//     });
// }

void ParticleSimulator::drawContent(sf::RenderWindow& window) {    
    
    // Draw Particles
    sf::CircleShape particleShape(particleRaduis);
    particleShape.setOrigin(particleRaduis/2,particleRaduis/2);

    // for (auto& particle : pm) {

    //     particleShape.setPosition(particle.position);
    //     particleShape.setFillColor(colorBlend(sf::Color::Cyan,sf::Color::Blue,
    //         (particle.density-targetdensity)/3));

    //     window.draw(particleShape);
    // }

    window.draw(pm);

    if (debug && !isMouseHeld) 
    // for (auto p = pm.begin(circle.getPosition()); p != pm.end(); ++p) {
    for (const Particle& p : pm.neighbors(circle.getPosition())) {

        particleShape.setPosition(p.getPosition());
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

void ParticleSimulator::setValue(std::string name, float value) {
    if (name == "smradius") {
        smradius = value;
        pm.setSmRadius(value);
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

