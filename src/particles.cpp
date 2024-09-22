#include "particles.hpp"

Particle::Particle(sf::Vector2f position, int squareID, idx next): 
    position(position), squareID(squareID), next(next) {}

Particles::Particles(int width, int height):
    width(width), height(height), parallel_for_each(particles) {

    // Load shader
    loadShader("gradient.frag");
    
    // Set Default Values
    setSmRadius(50);
    setParticleColor(sf::Color(0,200,255,150));
}

void Particles::setSmRadius(float smr) {

    smradius = smr;

    gridWidth = std::floor(width/smradius);
    gridHeight = std::floor(height/smradius);
    numSquares = gridWidth * gridHeight;

    squareWidth = float(width)/gridWidth;
    squareHeight = float(height)/gridHeight;

    headIdx.resize(numSquares);
    sortParticles();

    setShaderSmRadius(smr);
}

void Particles::setParticleColor(sf::Color color) {
    setShaderColor(color);
}

int Particles::getSquareID(sf::Vector2f pos) {

    if (std::isnan(pos.x) || std::isnan(pos.y)) 
        throw std::runtime_error("We lost one!");

    int x = std::min(int(pos.x/squareWidth), gridWidth-1);
    int y = std::min(int(pos.y/squareHeight), gridHeight-1);

    return y * gridWidth + x;
}

bool Particles::validCoords(int x, int y) {
    return 0 <= x && x < gridWidth && 0 <= y && y < gridHeight;
}

bool Particles::isValidSquareID(int squareID) {
    return 0 <= squareID && squareID < numSquares;
}

void Particles::addParticle(sf::Vector2f pos) {

    int squareID = getSquareID(pos);
    idx newIndex = particles.size();

    if (headIdx[squareID]) {
        particles[headIdx[squareID]].prev = newIndex;
    }
    
    particles.push_back(Particle(pos,squareID,headIdx[squareID]));
    headIdx[squareID] = newIndex;    
}

std::size_t Particles::size() {
    return particles.size();
}

void Particles::sortParticles() {

    for (int currIndex = 0; currIndex < particles.size(); ++currIndex) {

        Particle& p = particles[currIndex];
        int newSquareID = getSquareID(p.position);

        if (p.squareID != newSquareID) {

            // Detatch Node:
            // 1. Attatch prev to next, if no prev, make next head index
            if (p.prev)
                particles[p.prev].next = p.next;
            else
                headIdx[p.squareID] = p.next;
        
            // 2. Attatch next to prev if exists
            if (p.next) 
                particles[p.next].prev = p.prev;
            
            // Insert p to correct squareID
            if (headIdx[newSquareID])
                particles[headIdx[newSquareID]].prev = currIndex;

            p.next = headIdx[newSquareID];
            p.prev = idx();
            p.squareID = newSquareID;
            headIdx[newSquareID] = currIndex;
        }
    }
}

void Particles::moveParticles(float delta, float collision) {

    for (Particle& particle : particles) {

        sf::Vector2f currPos = particle.position;
        sf::Vector2 moveVec = delta * particle.velocity;
        sf::Vector2f newPos = currPos + moveVec;

        // Collision Logic
        while (newPos.x < 0 || newPos.x > width || newPos.y < 0 || newPos.y > height) {

            if (newPos.x < 0) {

                sf::Vector2f collisionPos(0,(-currPos.x)*moveVec.y/moveVec.x+currPos.y);

                if (0 <= collisionPos.y && collisionPos.y <= height) {

                    currPos = collisionPos;
                    moveVec = collision*(newPos-currPos);
                    moveVec.x *= -1;
                    particle.velocity.x *= - 1;
                }
            }
            
            if (newPos.x > width) {

                sf::Vector2f collisionPos(width,(width-currPos.x)*moveVec.y/moveVec.x+currPos.y);

                if (0 <= collisionPos.y && collisionPos.y <= height) {

                    currPos = collisionPos;
                    moveVec = collision*(newPos-currPos);
                    moveVec.x *= -1;
                    particle.velocity.x *= - 1;
                }
            }
            
            if (newPos.y < 0) {

                sf::Vector2f collisionPos((-currPos.y)*moveVec.x/moveVec.y+currPos.x,0);

                if (0 <= collisionPos.x && collisionPos.x <= width) {

                    currPos = collisionPos;
                    moveVec = collision*(newPos-currPos);
                    moveVec.y *= -1;
                    particle.velocity.y *= - 1;
                }
            }
            
            if (newPos.y > height) {

                sf::Vector2f collisionPos((height-currPos.y)*moveVec.x/moveVec.y+currPos.x,height);
                
                if (0 <= collisionPos.x && collisionPos.x <= width) {

                    currPos = collisionPos;
                    moveVec = collision*(newPos-currPos);
                    moveVec.y *= -1;
                    particle.velocity.y *= - 1;
                }
            }

            particle.velocity *= collision;
            newPos = currPos + moveVec;
        }

        particle.position = newPos;
    }

    sortParticles();
}

// Neigbor Iterator ///////////////////

Particles::Neighbors Particles::neighbors(sf::Vector2f pos) {
    return Neighbors(*this,pos);
}

Particles::Neighbors Particles::neighbors(Particle& p) {
    return Neighbors(*this,p.position);
}

Particles::Neighbors::Neighbors(Particles& pm):
    pm(pm), atEnd(true) {}

Particles::Neighbors::Neighbors(Particles& pm, sf::Vector2f pos):
    pm(pm) {

    int middle_x = std::min(int(pos.x/pm.squareWidth), pm.gridWidth-1);
    int middle_y = std::min(int(pos.y/pm.squareHeight), pm.gridHeight-1);

    // Add 9 neighboring squares
    if (pm.validCoords(middle_x,middle_y))
    for (int i = 0; i < 9; ++i) {
        int x = middle_x-1+i%3;
        int y = middle_y-1+i/3;
        if (pm.validCoords(x,y))
            closeHeadIdx[i] = pm.headIdx[y * pm.gridWidth + x];
    }

    this->operator++();
}

Particle& Particles::Neighbors::operator*() {
    return pm.particles[particleIdx];
}

Particles::Neighbors& Particles::Neighbors::operator++() {

    if (particleIdx)
        particleIdx = pm.particles[particleIdx].next;

    while (!particleIdx && n < 9)
        particleIdx = closeHeadIdx[n++];
    
    atEnd = !particleIdx;
    
    return *this;
}

bool Particles::Neighbors::operator!=(const Neighbors& other) const {
    return (!atEnd || !other.atEnd) &&
        (atEnd != other.atEnd || particleIdx != other.particleIdx);
}

Particles::Neighbors Particles::Neighbors::begin() {
    return *this;
}

Particles::Neighbors Particles::Neighbors::end() const {
    return Neighbors(pm);
}

// Drawing /////////////////

void Particles::loadShader(const char* file) {
    // For cross platform use
    while (!std::filesystem::exists("resources"))
    std::filesystem::current_path(std::filesystem::current_path().parent_path());

    if (!m_shader.loadFromFile(file, sf::Shader::Fragment))
        throw std::runtime_error("Failed to load shader");
}

void Particles::setShaderColor(sf::Color color) {
    m_shader.setUniform("c", sf::Glsl::Vec4(color));
}

void Particles::setShaderSmRadius(float smr) {
    m_shape.setSize(sf::Vector2f(2*smr,2*smr));
    m_shape.setOrigin(smr, smr);
    m_shader.setUniform("r", smr);
}

void Particles::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    
    if (drawGrid) {
        sf::RectangleShape r(sf::Vector2f(squareWidth,squareHeight));
        r.setFillColor(sf::Color(0,0,0,0));
        r.setOutlineColor(sf::Color::White);
        r.setOutlineThickness(1);

        for (int x = 0; x < gridWidth; ++x) {
            for (int y = 0; y < gridHeight; ++y) {
                r.setPosition(sf::Vector2f(x*squareWidth, y*squareHeight));
                target.draw(r,states);
            }
        }
    }
    
    states.shader = &m_shader;
    for (const Particle& p : particles) {
        m_shape.setPosition(p.position);
        m_shader.setUniform("p", sf::Glsl::Vec2(p.position.x,height-p.position.y));
        target.draw(m_shape, states);
    }
}
