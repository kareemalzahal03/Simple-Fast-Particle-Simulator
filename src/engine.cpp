#include "engine.hpp"

Engine::Engine():
    window(width,height,"Particle Simulator"),
    particlesim(width,height) {

    // For cross platform use
    while (!std::filesystem::exists("resources"))
    std::filesystem::current_path(std::filesystem::current_path().parent_path());

    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string command;
    while (std::getline(file, command)) {
        executeCommand(command);
    }

    file.close();
}

void Engine::run() {

    const sf::Time targetFrameTime = sf::microseconds(1000000/targetfps);
    while (window.isOpen()) {

        // Excecute frame and sleep until target time reached
        sf::Time frameTime = frame();
        accurateSleep(targetFrameTime - frameTime);
    }
}

sf::Time Engine::frame() {
    clock.restart();

    // Process Events
    processEvents();

    // Run Simulator Logic
    particlesim.simStep(1.f/targetfps);

    // Draw
    displayContent();

    return clock.getElapsedTime();
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

inline void Engine::displayContent() {

    window.clear(sf::Color(100,100,100));
    particlesim.drawContent(window);
    window.updateFPSText(targetfps);
    window.drawText();
    window.display();
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
    std::string name;
    float value;
    bool gotName = iss >> name ? true : false;
    bool gotValue = iss >> value ? true : false;

    // Read name and value from the command
    if (gotName && !gotValue) {

        // Pause, stop, etc.

    } else if (gotName && gotValue) {

        // Check any engine commands that takes values
        // otherwise, pass command to particlesim
        particlesim.setValue(name, value);

    } else if (!command.empty()) {

        std::cerr << "Invalid command!" << std::endl;
    }
}

inline void Engine::accurateSleep(sf::Time time) {
    clock.restart();
    while (time - clock.getElapsedTime() > sf::seconds(0)) {
        sf::sleep((time - clock.getElapsedTime())/2.f);
    }
}
