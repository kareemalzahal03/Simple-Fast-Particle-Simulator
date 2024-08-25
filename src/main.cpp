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

struct AccurateSleep {
    void operator()(sf::Time time) {
        clock.restart();
        sf::sleep(time - diff);
        diff += clock.getElapsedTime() - time;
    }
private:
    sf::Time diff;
    sf::Clock clock;
};

int main() {

    Window window("Particle Simulator");
    ParticleSimulator particlesim;
    AccurateSleep accuratesleep;

    sf::Clock deltaClock;
    sf::Clock drawClock;
    sf::Time frameTime;

    while (window.isOpen()) {

        drawClock.restart();
        // Poll Events
        sf::Event event;
        while (window.pollEvent(event)) { 
            window.onEvent(event);
            particlesim.onEvent(event, window.getSize());
        }
        // Draw
        window.clear(sf::Color(100,100,100));
        particlesim.drawContent(window);
        window.drawFPS();
        window.display();

        particlesim.simStep(1.f/fps);
        frameTime = deltaClock.restart();

        window.updateFPS(frameTime);
        accuratesleep(sf::milliseconds(1000/fps) - frameTime);
    }

    return 0;
}
