#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

// WINDOW

const int WIDTH = 1450;
const int HEIGHT = 900;
const int MAX_FRAME_RATE = 1000;
const sf::Time FPS_REFRESH_RATE = sf::seconds(1.0f);

// SIMULATOR

const int PARTICLE_RADIUS = 6;

const float VISCOSITY = 0.75;

const int particleSpawn = 1500;
const float smoothingRadius = 50;

const float gravity = 4000;
const float mass = 1;
const float targetDensity = 3;
const float pressureMultiplier = 3000000;
const float mouseForces = 10000;
const float collisionLoss = 0.5;

// const float frictionMultiplier = 1;

const int MOUSE_RADIUS = 100;

// const float SpikyPow2ScalingFactor = 6 / (M_PI * std::pow(smoothingRadius, 4));
// const float SpikyPow2DerivativeScalingFactor = - 12 / (M_PI * std::pow(smoothingRadius, 4));

// COLORS

const sf::Color defaultColor = sf::Color::Cyan;
const sf::Color repelColor = sf::Color::Red;
const sf::Color attractColor = sf::Color::Green;
