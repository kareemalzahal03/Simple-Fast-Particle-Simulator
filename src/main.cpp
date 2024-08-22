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
    bool debug = false;

    while (window.isOpen()) {

        // Poll Events
        sf::Event event;
        while (window.pollEvent(event)) { 
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::D) debug = !debug;
            } else {
                particlesim.onMouseEvent(event);
            }
        }

        // Run Simulation Logic
        particlesim.simStep(window.calculateDelta());

        // Draw
        window.clear(sf::Color(100,100,100));
        particlesim.drawContent(window,debug);
        window.drawFPS();
        window.display();
    }

    return 0;
}
