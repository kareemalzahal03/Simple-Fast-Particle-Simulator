#include "config.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>

Config::Config() {
    
    // For cross platform use
    while (!std::filesystem::exists("resources"))
    std::filesystem::current_path(std::filesystem::current_path().parent_path());

    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string command;
    while (std::getline(file, command)) {
        this->operator()(command);
    }

    file.close();
}

void Config::set(std::string name, float value) {
    for (int i = 0; i < varcount; ++i) {
        if (name == names[i]) {
            values[i] = value;
            return;
        }
    }
    printError();
}

void Config::printError() {
    std::cerr << "Invalid command. Usage: <string name> <float value>" << std::endl;
}

void Config::operator()(std::string command) {
    std::istringstream iss(command);
    std::string name;
    float value;

    // Read name and value from the command
    if (iss >> name >> value) {
        set(name, value);
    } else if (!command.empty()) {
        printError();
    }
}

Config& Config::get() {
    static Config instance;
    return instance;
}
