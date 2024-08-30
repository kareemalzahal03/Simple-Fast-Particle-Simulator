#include <SFML/Graphics.hpp>
#include <cmath>

sf::Color colorBlend(sf::Color c1, sf::Color c2, float blendRatio);

float CosKernel(float dst, float radius);

float SpikyKernelPow2(float dst, float radius);

float DerivativeSpikyPow2(float dst, float radius);

float SharedPressure(float density1, float density2, float targetdensity, float pressure);

float Magnitude(sf::Vector2f v);

sf::Vector2f Unit(sf::Vector2f v);