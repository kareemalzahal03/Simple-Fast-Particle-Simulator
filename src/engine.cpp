#include "engine.hpp"

Engine::Engine():
    window(width,height,"Particle Simulator"), particlesim(width,height) {}

void Engine::run() {

    while (window.isOpen()) {

        sf::Time frameTime = frame();

        window.updateFPSText(targetfps);
        accurateSleep(sf::microseconds(1000000/targetfps) - frameTime);
    }
}

sf::Time Engine::frame() {
    clock.restart();

    // Process Events
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            onKeyPressed(event.key.code);
        }
        particlesim.onEvent(event, window.getSize());
    }

    // Draw
    window.clear(sf::Color(100,100,100));
    particlesim.drawContent(window);
    window.drawText();
    window.display();

    // Run Sim Step
    particlesim.simStep(1.f/targetfps);

    return clock.restart();
}

void Engine::interpretCommand() {

    Config& config = Config::get();
    config(command);
    command.clear();
}

void Engine::onKeyPressed(sf::Keyboard::Key code) {
    
    if (0 <= code && code <= 25)
        command += 'a'+code;
    else if (26 <= code && code <= 35)
        command += '0'-26+code;
    else if (code == sf::Keyboard::Period)
        command += '.';
    else if (code == sf::Keyboard::Space)
        command += ' ';
    else if (code == sf::Keyboard::Backspace && !command.empty())
        command.pop_back();
    else if (code == sf::Keyboard::Enter)
        interpretCommand();

    window.updateCommandText(command);
}

void Engine::accurateSleep(sf::Time time) {
    clock.restart();
    while (time - clock.getElapsedTime() > sf::seconds(0)) {
        sf::sleep((time - clock.getElapsedTime())/2.f);
    }
}
