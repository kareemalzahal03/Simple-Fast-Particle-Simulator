#include <SFML/Graphics.hpp>
#include "config.hpp"
#include <cmath>

sf::Color colorBlend(sf::Color c1, sf::Color c2, float blendRatio) {
    blendRatio = std::max(std::min(blendRatio,1.0f),0.0f);
    return sf::Color(
    (c1.r*(1-blendRatio))+(c2.r*blendRatio),
    (c1.g*(1-blendRatio))+(c2.g*blendRatio),
    (c1.b*(1-blendRatio))+(c2.b*blendRatio));
}

float CosKernel(float dst, float radius) {

    if (dst > radius) { return 0; }

    float x = radius - dst;

    return 0.5 * (std::cos((x/radius)*3.1415926) + 1);
}

float SpikyKernelPow2(float dst, float radius) {

    if (dst > radius) { return 0; }

    float v = (radius - dst);

    return (v*v)/((radius)*(radius));
}

float DerivativeSpikyPow2(float dst, float radius) {

    if (dst > radius) { return 0; }

    float v = (radius - dst);

    return - 2 * v / ((radius)*(radius));
}

float DensityToPressure(float density) {
    Config& config = Config::get();

    return (density - config.targetdensity()) * config.pressure();
}

float SharedPressure(float density1, float density2) {

    return (DensityToPressure(density1) + DensityToPressure(density2)) / 2;
}

float Magnitude(sf::Vector2f v) {

    return std::sqrt(v.x*v.x+v.y*v.y);
}

sf::Vector2f Unit(sf::Vector2f v) {
    
    float mag = Magnitude(v);
    if (mag == 0) {return sf::Vector2f(0,0);}
    return v / mag;
}