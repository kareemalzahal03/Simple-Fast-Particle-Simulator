#include "particle.hpp"
#include "window.hpp"
#include <iostream>

Particle::Particle() {
    
    this->setRadius(PARTICLE_RADIUS);
    this->setFillColor(defaultColor);
    this->setOrigin(getRadius()/2,getRadius()/2);

    // Set Random Position
    this->setPosition(sf::Vector2f(
        static_cast <float> (rand ()) / static_cast <float> (RAND_MAX) * WIDTH, 
        static_cast <float> (rand ()) / static_cast <float> (RAND_MAX) * HEIGHT));
}

void Particle::moveParticle(float delta) {
    
    sf::Vector2f currPos = this->getPosition();
    sf::Vector2 moveVec = delta * velocity;
    sf::Vector2f newPos = currPos + moveVec;

    while (newPos.x < 0 or newPos.x > WIDTH or newPos.y < 0 or newPos.y > HEIGHT) {

        if (newPos.x < 0) {

            sf::Vector2f collisionPos(0,(-currPos.x)*moveVec.y/moveVec.x+currPos.y);

            if (0 <= collisionPos.y and collisionPos.y <= HEIGHT) {

                currPos = collisionPos;
                moveVec = collisionLoss*(newPos-currPos);
                moveVec.x *= -1;
                velocity.x *= - 1;
            }
        }
        
        if (newPos.x > WIDTH) {

            sf::Vector2f collisionPos(WIDTH,(WIDTH-currPos.x)*moveVec.y/moveVec.x+currPos.y);

            if (0 <= collisionPos.y and collisionPos.y <= HEIGHT) {

                currPos = collisionPos;
                moveVec = collisionLoss*(newPos-currPos);
                moveVec.x *= -1;
                velocity.x *= - 1;
            }
        }
        
        if (newPos.y < 0) {

            sf::Vector2f collisionPos((-currPos.y)*moveVec.x/moveVec.y+currPos.x,0);

            if (0 <= collisionPos.x and collisionPos.x <= WIDTH) {

                currPos = collisionPos;
                moveVec = collisionLoss*(newPos-currPos);
                moveVec.y *= -1;
                velocity.y *= - 1;
            }
        }
        
        if (newPos.y > HEIGHT) {

            sf::Vector2f collisionPos((HEIGHT-currPos.y)*moveVec.x/moveVec.y+currPos.x,HEIGHT);
            
            if (0 <= collisionPos.x and collisionPos.x <= WIDTH) {

                currPos = collisionPos;
                moveVec = collisionLoss*(newPos-currPos);
                moveVec.y *= -1;
                velocity.y *= - 1;
            }
        }

        velocity *= collisionLoss;
        newPos = currPos + moveVec;
    }

    this->setPosition(newPos);
}
