#pragma once
#include "parallelize.hpp"
#include "particle.hpp"
#include <vector>
#include <list>

class ParticleManager {
friend class ParticleIterator;
public:

    ParticleManager(int width, int height);
    void addParticle(Particle particle);
    void sortParticles();
    void drawBoxes(sf::RenderWindow& window);
    void updateSmRadius(float smr);

    class ParticleIterator {
    friend class ParticleManager;
    public:

        bool operator!=(const ParticleIterator& other) const;
        Particle& operator*();
        Particle* operator->();
        ParticleIterator& operator++();

    private:

        ParticleIterator();
        ParticleIterator(ParticleManager* pm);
        ParticleIterator(ParticleManager* pm, sf::Vector2f pos);
        void advanceToValid();

        ParticleManager* pm;
        std::list<int> closeSquareIDs;
        bool isEnd = false;
        bool isRegional = false;
        int nodeIndex = -1;
    };

    ParticleIterator begin();
    ParticleIterator end();
    ParticleIterator begin(sf::Vector2f pos);
    int size();

    template <typename Func>
    void parallel_for_each(Func func) {
        parallelize([this, &func](int threadIndex, int threadCount){

            size_t partSize = particles.size() / threadCount;
            size_t start = threadIndex * partSize;
            size_t end = (threadIndex == threadCount - 1) ? particles.size() : start + partSize;

            for (size_t i = start; i < end; ++i) {
                func(particles[i].particle);
            }
        });
    }

private:

    int getSquareID(sf::Vector2f pos);
    bool isValidSquareID(int squareID);
    std::list<int> getCloseSquareIDs(sf::Vector2f pos);
    Parallelize parallelize;

    const int width;
    const int height;

    float smradius;
    float squareWidth;
    float squareHeight;
    int numSquares;
    int gridWidth;
    int gridHeight;

    struct PNode {
        PNode(Particle particle, int squareID, 
            int next = -1, int prev = -1);
        Particle particle;
        int squareID;
        int next;
        int prev;
    };

    std::vector<PNode> particles;
    std::vector<int> sidHeadIndecies;

    // std::vector<std::list<Particle>> particlesInSquare;
};
