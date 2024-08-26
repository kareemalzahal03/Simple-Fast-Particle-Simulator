#pragma once
#include "particle.hpp"
#include "config.hpp"
#include <vector>
#include <list>

class ParticleManager {
friend class ParticleIterator;
public:

    ParticleManager(int width, int height);
    void addParticle(Particle particle);
    void sortParticles();
    void drawBoxes(sf::RenderWindow& window);

    class ParticleIterator {
    friend class ParticleManager;
    public:

        bool operator!=(const ParticleIterator& other) const;
        Particle& operator*();
        Particle* operator->();
        ParticleIterator& operator++();
        void erase();

    private:

        ParticleIterator();
        ParticleIterator(ParticleManager* pm);
        ParticleIterator(ParticleManager* pm, sf::Vector2f pos);
        void advanceToValid();

        ParticleManager* pm;
        std::list<int> closeSquareIDs;
        bool isEnd = false;
        bool isRegional = false;
        int currentSquareID = 0;
        std::list<Particle>::iterator particleIT;
    };

    ParticleIterator begin();
    ParticleIterator end();
    ParticleIterator begin(sf::Vector2f pos);
    int size();

private:

    int getSquareID(sf::Vector2f pos);
    bool isValidSquareID(int squareID);
    std::list<int> getCloseSquareIDs(sf::Vector2f pos);
    void updateGridVariables();
    bool outdatedGridVariables();
    void sort();

    const int width;
    const int height;

    int particleCount = 0;
    
    float smradius;
    float squareWidth;
    float squareHeight;
    int numSquares;
    int gridWidth;
    int gridHeight;

    std::vector<std::list<Particle>> particlesInSquare;
};
