#pragma once
#include <SFML/Graphics.hpp>
#include "parforeach.hpp"
#include "idx.hpp"
#include <vector>
#include <iostream>
#include <cmath>
#include <filesystem>

class Particle {
public:

    sf::Vector2f velocity;
    sf::Vector2f force;
    float density;

    sf::Vector2f getPosition() const { return position; }

private:
friend class Particles;

    explicit Particle(sf::Vector2f position, int squareID, idx next);
    sf::Vector2f position;
    int squareID;
    idx next;
    idx prev;
};

class Particles : public sf::Drawable {
public:

    Particles(int width, int height);

    void addParticle(sf::Vector2f pos);
    void moveParticles(float delta, float collision);
    void setSmRadius(float smr);
    void setParticleColor(sf::Color color);
    std::size_t size();
    ParForEach<Particle> parallel_for_each;

    // Iterators

    auto begin() { return particles.begin(); }
    auto end() { return particles.end(); }

    class Neighbors {
    public:

        Particle& operator*();
        Neighbors& operator++();
        bool operator!=(const Neighbors& other) const;
        Neighbors begin();
        Neighbors end() const;

    private:
    friend Particles;

        Neighbors(Particles& pm);
        Neighbors(Particles& pm, sf::Vector2f pos);

        idx closeHeadIdx[9];
        int n = 0;
        Particles& pm; 
        idx particleIdx;
        bool atEnd;
    };

    Neighbors neighbors(sf::Vector2f pos);
    Neighbors neighbors(Particle& p);

private:

    int getSquareID(sf::Vector2f pos);
    void sortParticles(); //
    bool validCoords(int x, int y);
    bool isValidSquareID(int squareID);

    const int width;
    const int height;

    float smradius;
    float squareWidth;
    float squareHeight;
    int numSquares;
    int gridWidth;
    int gridHeight;

    std::vector<Particle> particles;
    std::vector<idx> headIdx;

    // Drawing

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void loadShader(const char* file);
    void setShaderColor(sf::Color color);
    void setShaderSmRadius(float smr);

    bool drawGrid = false;
    mutable sf::Shader m_shader;
    mutable sf::RectangleShape m_shape;
};
