#include "engine.hpp"

Engine::Engine(int width, int height, const char* configfile):
    window(width,height,"Particle Simulator"),
    particlesim(width,height) {

    // For cross platform use
    while (!std::filesystem::exists("resources"))
    std::filesystem::current_path(std::filesystem::current_path().parent_path());

    // Load Config File
    std::ifstream file(configfile);
    if (!file) throw std::runtime_error("Error opening config file!");
    std::string command;
    while (std::getline(file, command)) executeCommand(command);
    file.close();
}

void Engine::run() {

    particlesim.addParticles(spawn);

    std::thread t([this](){

        while (window.isOpen()) {
            frameClock.restart();
            frame();
            sf::Time frameTime = frameClock.getElapsedTime();
            sf::Time targetFrameTime = sf::microseconds(1000000/targetFPS);
            accurateSleep(targetFrameTime - frameTime, frameClock);
            window.updateFPSText();
        }
    });

    while (window.isOpen()) {

        // Excecute step and sleep until target time reached
        stepClock.restart();
        step();
        sf::Time stepTime = stepClock.getElapsedTime();
        sf::Time targetStepTime = sf::microseconds(1000000/targetSPS);
        accurateSleep(targetStepTime - stepTime, stepClock);
        window.updateSPSText();
    }

    t.join();
}

inline void Engine::step() {

    // Process Events
    processEvents();

    // Run Simulator Logic
    particlesim.simStep(1.f/targetSPS);
}

inline void Engine::frame() {

    // Draw to window
    window.clear();
    window.drawBackground();
    particlesim.drawContent(window);
    window.drawText(targetFPS, targetSPS);
    window.display();
}

inline void Engine::processEvents() {

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            onKeyPressed(event.key.code);
        }
        particlesim.onEvent(event, window.getSize());
    }
}

inline void Engine::onKeyPressed(sf::Keyboard::Key code) {

    if (sf::Keyboard::A <= code && code <= sf::Keyboard::Z)
        command += 'a'+code;
    else if (sf::Keyboard::Num0 <= code && code <= sf::Keyboard::Num9)
        command += '0'+code-sf::Keyboard::Num0;
    else if (code == sf::Keyboard::Period)
        command += '.';
    else if (code == sf::Keyboard::Space)
        command += ' ';
    else if (code == sf::Keyboard::Backspace && !command.empty())
        command.pop_back();
    else if (code == sf::Keyboard::Enter) {
        executeCommand(command);
        command.clear();
    }

    window.updateCommandText(command);
}

inline void Engine::executeCommand(std::string command) {

    std::istringstream iss(command);
    std::string varname;
    float value;
    bool gotVarName = iss >> varname ? true : false;
    bool gotValue = iss >> value ? true : false;

    if (gotVarName && gotValue) {

        if (varname == "spawn") {
            spawn = value;
        } else if (varname == "targetsps") {
            targetSPS = value;
        } else if (varname == "targetfps") {
            targetFPS = value;
        } else {
            particlesim.setValue(varname, value);
        }

    } else if (!command.empty()) {

        std::cerr << "Invalid command!" << std::endl;
    }
}

inline void Engine::accurateSleep(sf::Time time, sf::Clock& clock) {
    clock.restart();
    while (time - clock.getElapsedTime() > sf::seconds(0)) {
        sf::sleep((time - clock.getElapsedTime())/2.f);
    }
}
