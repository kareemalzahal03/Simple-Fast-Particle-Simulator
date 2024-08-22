#include "particle.hpp"
#include "particlemanager.hpp"
#include <vector>
#include <iostream>
#include <cmath>

ParticleManager::ParticleManager(int width, int height, int smradius) {
    Config& config = Config::get();

    squareSize = smradius;
    gridWidth = width/squareSize;
    gridHeight = height/squareSize;
    numSquares = gridWidth * gridHeight;

    particlesInSquare.resize(numSquares);
}

int ParticleManager::getSquareID(sf::Vector2f pos) {

    if (std::isnan(pos.x) || std::isnan(pos.y)) return -1000;

    int x = pos.x/squareSize;
    int y = pos.y/squareSize;    
    y -= (y == gridHeight);
    x -= (x == gridWidth);

    return y * gridWidth + x;
}


std::list<int> ParticleManager::getCloseSquareIDs(sf::Vector2f pos) {
    std::list<int> closeSquareIDs;

    int x = pos.x/squareSize; x -= (x == gridWidth);
    int y = pos.y/squareSize; y -= (y == gridHeight);

    if (!isValidSquareID(y * gridWidth + x)) return {};

    for (int w = x-1; w <= x+1; ++w) {
        for (int h = y-1; h <= y+1; ++h) {
            if (0 <= w && w < gridWidth && 0 <= h && h < gridHeight)
                closeSquareIDs.push_back(h * gridWidth + w);
        }
    }

    return closeSquareIDs;
}

bool ParticleManager::isValidSquareID(int squareID) {
    return 0 <= squareID && squareID < numSquares;
}

void ParticleManager::addParticle(Particle p) {

    particlesInSquare[getSquareID(p.getPosition())].push_front(p);
    particleCount++;
}

int ParticleManager::size() {
    return particleCount;
}

void ParticleManager::sortParticles() {
    for (int sID = 0; sID < numSquares; ++sID) {
        for (auto it = particlesInSquare[sID].begin(); it != particlesInSquare[sID].end(); ++it) {
            int newSquareID = getSquareID(it->getPosition());
            
            if (!isValidSquareID(newSquareID)) {

                it = particlesInSquare[sID].erase(it);
                particleCount--;
                std::cout << "VANISH!\n";
                continue;

            } else if (sID != newSquareID) {

                particlesInSquare[newSquareID].push_front(*it);
                it = particlesInSquare[sID].erase(it);
                continue;
            }
        }
    }
}

// Iterator ///////////////////

void ParticleManager::ParticleIterator::advanceToValid() {

    while (isEnd == false && particleIT == pm->particlesInSquare[currentSquareID].end()) {

        if (isRegional) {

            if (closeSquareIDs.empty()) {

                isEnd = true;

            } else {

                currentSquareID = closeSquareIDs.front();
                particleIT = pm->particlesInSquare[currentSquareID].begin();
                closeSquareIDs.pop_front();
            }

        } else {

            if (currentSquareID == pm->numSquares - 1) {

                isEnd = true;

            } else {

                currentSquareID++;
                particleIT = pm->particlesInSquare[currentSquareID].begin();
            }
        }
    }
}

bool ParticleManager::ParticleIterator::operator!=(const ParticleIterator& other) const {

    if (isEnd && other.isEnd) return false;
    else if (isEnd || other.isEnd) return true;
    else return particleIT != other.particleIT;
}

Particle& ParticleManager::ParticleIterator::operator*() {
    return *particleIT;
}

Particle* ParticleManager::ParticleIterator::operator->() {
    return &(*particleIT);
}

ParticleManager::ParticleIterator& ParticleManager::ParticleIterator::operator++() {
    ++particleIT;
    advanceToValid();
    return *this;
}

void ParticleManager::ParticleIterator::erase() {
    particleIT = pm->particlesInSquare[currentSquareID].erase(particleIT);
    advanceToValid();
}

ParticleManager::ParticleIterator::ParticleIterator(ParticleManager* pm, sf::Vector2f pos):
pm{pm}, isRegional{true}, closeSquareIDs{pm->getCloseSquareIDs(pos)}, isEnd{closeSquareIDs.empty()} {
    
    if (isEnd) return;

    currentSquareID = closeSquareIDs.front();
    particleIT = pm->particlesInSquare[currentSquareID].begin();
    closeSquareIDs.pop_front();
    advanceToValid();
}

ParticleManager::ParticleIterator::ParticleIterator(ParticleManager* pm):
pm{pm}, particleIT{pm->particlesInSquare[0].begin()} { 
    
    advanceToValid();
}

ParticleManager::ParticleIterator::ParticleIterator(): isEnd{true} {}

ParticleManager::ParticleIterator ParticleManager::begin(sf::Vector2f pos) {
    return ParticleIterator(this, pos);
}

ParticleManager::ParticleIterator ParticleManager::begin() {
    return ParticleIterator(this);
}

ParticleManager::ParticleIterator ParticleManager::end() {
    return ParticleIterator();
}
