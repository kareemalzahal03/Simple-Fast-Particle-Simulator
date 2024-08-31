#include "particle.hpp"
#include "particlemanager.hpp"
#include <vector>
#include <iostream>
#include <cmath>

ParticleManager::ParticleManager(int width, int height):
    width(width), height(height) {

    updateSmRadius(50);
}

ParticleManager::PNode::PNode(Particle particle, int squareID, int next, int prev):
    particle(particle), squareID(squareID), next(next), prev(prev) {}

void ParticleManager::updateSmRadius(float smr) {

    smradius = smr;

    gridWidth = std::floor(width/smradius);
    gridHeight = std::floor(height/smradius);
    numSquares = gridWidth * gridHeight;

    squareWidth = float(width)/gridWidth;
    squareHeight = float(height)/gridHeight;

    sidHeadIndecies.resize(numSquares,-1);
    sortParticles();
}

int ParticleManager::getSquareID(sf::Vector2f pos) {

    if (std::isnan(pos.x) || std::isnan(pos.y)) 
        throw std::runtime_error("Shit! We lost one!");

    int x = pos.x/squareWidth;
    int y = pos.y/squareHeight;    
    y -= (y == gridHeight);
    x -= (x == gridWidth);

    return y * gridWidth + x;
}

std::list<int> ParticleManager::getCloseSquareIDs(sf::Vector2f pos) {
    std::list<int> closeSquareIDs;

    int x = pos.x/squareWidth; x -= (x == gridWidth);
    int y = pos.y/squareHeight; y -= (y == gridHeight);

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

    int squareID = getSquareID(p.position);
    int index = particles.size();

    if (sidHeadIndecies[squareID] != -1) {
        particles[sidHeadIndecies[squareID]].prev = index;
    }
    
    particles.push_back(PNode(p,squareID,sidHeadIndecies[squareID]));
    sidHeadIndecies[squareID] = index;
}

int ParticleManager::size() {
    return particles.size();
}

void ParticleManager::sortParticles() {

    for (int currIndex = 0; currIndex < particles.size(); ++currIndex) {

        PNode& currNode = particles[currIndex];
        int newSquareID = getSquareID(currNode.particle.position);

        if (currNode.squareID != newSquareID) {

            // Detatch Node:
            // 1. Attatch prev to next, if no prev, make next head index
            if (currNode.prev != -1) {

                particles[currNode.prev].next = currNode.next;

            } else {

                sidHeadIndecies[currNode.squareID] = currNode.next;
            }
            // 2. Attatch next to prev if exists
            if (currNode.next != -1) {

                particles[currNode.next].prev = currNode.prev;
            }

            // Insert currNode to correct squareID

            if (sidHeadIndecies[newSquareID] != -1) {

                particles[sidHeadIndecies[newSquareID]].prev = currIndex;
            }

            currNode.next = sidHeadIndecies[newSquareID];
            currNode.prev = -1;
            currNode.squareID = newSquareID;
            sidHeadIndecies[newSquareID] = currIndex;
        }
    }



    // for (int currSquareID = 0; currSquareID < sidHeadIndecies.size(); currSquareID++) {

    //     for (int currIndex = sidHeadIndecies[currSquareID]; currIndex != -1; currIndex = particles[currIndex].next) {
        
    //         PNode& currNode = particles[currIndex];
    //         int newSquareID = getSquareID(currNode.particle.position);

    //         if (currSquareID != newSquareID) {

    //             // Detatch Node:
    //             // 1. Attatch prev to next, if no prev, make next head index
    //             if (currNode.prev != -1) {

    //                 particles[currNode.prev].next = currNode.next;

    //             } else {

    //                 sidHeadIndecies[currSquareID] = currNode.next;
    //             }
    //             // 2. Attatch next to prev if exists
    //             if (currNode.next != -1) {

    //                 particles[currNode.next].prev = currNode.prev;
    //             }

    //             // Insert currNode to correct squareID

    //             if (sidHeadIndecies[newSquareID] != -1) {

    //                 particles[sidHeadIndecies[newSquareID]].prev = currIndex;
    //             }

    //             currNode.next = sidHeadIndecies[newSquareID];
    //             currNode.prev = -1;
    //             sidHeadIndecies[newSquareID] = currIndex;
    //         }
    //     }
    // }
}

void ParticleManager::drawBoxes(sf::RenderWindow& window) {

    sf::RectangleShape r(sf::Vector2f(squareWidth,squareHeight));
    r.setFillColor(sf::Color(0,0,0,0));
    r.setOutlineColor(sf::Color::White);
    r.setOutlineThickness(1);

    for (int x = 0; x < gridWidth; ++x) {
        for (int y = 0; y < gridHeight; ++y) {
            r.setPosition(sf::Vector2f(x*squareWidth, y*squareHeight));
            window.draw(r);
        }
    }
}

// Iterator ///////////////////

bool ParticleManager::ParticleIterator::operator!=(const ParticleIterator& other) const {

    if (isEnd && other.isEnd) return false;
    else if (isEnd || other.isEnd) return true;
    else return nodeIndex != other.nodeIndex;
}

Particle& ParticleManager::ParticleIterator::operator*() {
    return pm->particles[nodeIndex].particle;
}

Particle* ParticleManager::ParticleIterator::operator->() {
    return &(pm->particles[nodeIndex].particle);
}

ParticleManager::ParticleIterator& ParticleManager::ParticleIterator::operator++() {
    
    if (isEnd) throw std::runtime_error("operator++ used on end()!");

    if (isRegional) {

        nodeIndex = pm->particles[nodeIndex].next;

        while (nodeIndex == -1 && !closeSquareIDs.empty()) {
            nodeIndex = pm->sidHeadIndecies[closeSquareIDs.front()];
            closeSquareIDs.pop_front();
        }

        isEnd = (nodeIndex == -1);

    } else {

        nodeIndex++;
        isEnd = (nodeIndex == pm->particles.size());
    }

    return *this;
}

ParticleManager::ParticleIterator::ParticleIterator(ParticleManager* pm, sf::Vector2f pos):
pm{pm}, isRegional{true}, closeSquareIDs{pm->getCloseSquareIDs(pos)} {
    
    while (nodeIndex == -1 && !closeSquareIDs.empty()) {
        nodeIndex = pm->sidHeadIndecies[closeSquareIDs.front()];
        closeSquareIDs.pop_front();
    }

    isEnd = (nodeIndex == -1);
}

ParticleManager::ParticleIterator::ParticleIterator(ParticleManager* pm):
pm{pm}, nodeIndex{0}, isEnd{pm->particles.empty()} {}

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
