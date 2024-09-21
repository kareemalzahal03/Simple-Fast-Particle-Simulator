#pragma once
#include <SFML/Graphics.hpp>
#include "parallelize.hpp"
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
friend class ParticleManager;

    explicit Particle(sf::Vector2f position, int squareID, idx next);
    sf::Vector2f position;
    int squareID;
    idx next;
    idx prev;
};

class ParticleManager : public sf::Drawable {
public:

    ParticleManager(int width, int height);
    void addParticle(sf::Vector2f pos);
    
    void moveParticles(float delta, float collision);
    void setSmRadius(float smr);
    void setParticleColor(sf::Color color);
    
    std::size_t size();

    template <typename Func>
    void parallel_for_each(Func func) {
        parallelize([this, &func](int threadIndex, int threadCount){

            size_t partSize = particles.size() / threadCount;
            size_t start = threadIndex * partSize;
            size_t end = (threadIndex == threadCount - 1) ? particles.size() : start + partSize;

            for (size_t i = start; i < end; ++i) {
                func(particles[i]);
            }
        });
    }

    // Neighbor Iterator

    class Neighbors {
    public:

        Particle& operator*();
        Neighbors& operator++();
        bool operator!=(const Neighbors& other) const;
        Neighbors begin();
        Neighbors end() const;

    private:
    friend ParticleManager;

        Neighbors(ParticleManager& pm);
        Neighbors(ParticleManager& pm, sf::Vector2f pos);

        idx closeHeadIdx[9];
        int n = 0;
        ParticleManager& pm; 
        idx particleIdx;
        bool atEnd;
    };

    Neighbors neighbors(sf::Vector2f pos);
    Neighbors neighbors(Particle& p);

    auto begin() { return particles.begin(); }
    auto end() { return particles.end(); }

private:

    int getSquareID(sf::Vector2f pos);
    // bool isValidPos(sf::Vector2f pos);
    void sortParticles(); //
    bool validCoords(int x, int y);
    bool isValidSquareID(int squareID);
    Parallelize parallelize;

    const int width;
    const int height;

    float smradius;
    float squareWidth;
    float squareHeight;
    int numSquares;
    int gridWidth;
    int gridHeight;


    // struct ParticleNode {
    //     int squareID;
    //     idx next;
    //     idx prev;
    // };

    std::vector<Particle> particles;
    // std::vector<ParticleNode> nodes;
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
