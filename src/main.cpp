///////////// © 2024 KAREEM ALZAHAL /////////////
//                                             //
//    ┏┓┏┓┳┓┏┳┓┳┏┓┓ ┏┓  ┏┓┳┏┳┓┓┏┓ ┏┓┏┳┓┏┓┳┓    //
//    ┣┛┣┫┣┫ ┃ ┃┃ ┃ ┣   ┗┓┃┃┃┃┃┃┃ ┣┫ ┃ ┃┃┣┫    //
//    ┗ ┛┗┛┗ ┻ ┻┗┛┗┛┗┛  ┗┛┻┛ ┗┗┛┗┛┛┗ ┻ ┗┛┛┗    //
//                                             //
/////////////////////////////////////////////////

#include <SFML/Graphics.hpp>
#include "window.hpp"
#include "particlesim.hpp"
#include <iostream>

int main() {
    const int width = 1450;
    const int height = 900;
    const int smradius = 50;

    Window window(width, height, "Particle Simulator");
    ParticleSimulator particlesim(width,height,smradius);
    sf::Clock deltaClock;
    float delta = 0;

    while (window.isOpen()) {

        deltaClock.restart();

        // Run Simulation Logic
        particlesim.simStep(delta);

        // Draw
        window.clear(sf::Color(100,100,100));
        particlesim.drawContent(window);
        window.drawFPS(delta);
        window.display();

        delta = deltaClock.restart().asSeconds();

        // Poll Events
        sf::Event event;
        while (window.pollEvent(event)) { 

            window.onEvent(event);
            particlesim.onEvent(event);
        }
    }

    return 0;
}
