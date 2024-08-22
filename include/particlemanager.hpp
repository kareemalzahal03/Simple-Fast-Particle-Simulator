#pragma once
#include "particle.hpp"
#include "config.hpp"
#include <vector>
#include <list>

class ParticleManager {
friend class ParticleIterator;
public:

    ParticleManager(int width, int height, int smradius);
    void addParticle(Particle particle);
    void sortParticles();

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

    int particleCount = 0;
    int squareSize;
    int numSquares;
    int gridWidth;
    int gridHeight;

    std::vector<std::list<Particle>> particlesInSquare;
};
