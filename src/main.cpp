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
    Config& config = Config::get();
    int width = config.width();
    int height = config.height();

    Window window(width,height,"Particle Simulator");
    ParticleSimulator particlesim(width,height,config.smradius());
    
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
