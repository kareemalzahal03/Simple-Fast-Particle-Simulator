#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

// Attempts to sleep for an accurate amount of time
struct AccurateSleep {
    void operator()(sf::Time time) {
        // Previous:
        // clock.restart();
        // sf::sleep(time - diff);
        // diff += clock.restart() - time;
        clock.restart();
        while (time - clock.getElapsedTime() > sf::seconds(0)) {
            sf::sleep((time - clock.getElapsedTime())/2.f);
        }
    }
private:
    sf::Time diff;
    sf::Clock clock;
};
