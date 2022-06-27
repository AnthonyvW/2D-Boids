
#include "pch.h"
#include "boid.h"
#include <iostream>

extern const int screenWidth;
extern const int screenHeight;

float range = 30;
float maxAcceleration = 0.3;
float maxSpeed = 4;

float seperationStrength = 0.4;
float cohesionStrength = 0.3;
float alignmentStrength = 0.5;

boid::boid() {
    position = sf::Vector2f(0, 0);
    velocity = sf::Vector2f(0, 0);
    acceleration = sf::Vector2f(0, 0);

    id = 0;
};

boid::boid(float newx, float newy, float xDir, float yDir, int newID) {
    position = sf::Vector2f(newx, newy);
    velocity = sf::Vector2f(xDir, yDir);
    acceleration = sf::Vector2f(0, 0);

    id = newID;
}

sf::Vector2f boid::getPos() { return position; }
sf::Vector2f boid::getVel() { return velocity; }
sf::Vector2f boid::getAcc() { return acceleration; }

int boid::getID() { return id; }

float vect2fDist(sf::Vector2f pos1, sf::Vector2f pos2) {
    float x = pos1.x - pos2.x;
    float y = pos1.y - pos2.y;
    return x * x + y * y;
}

// A Reminder for how steering works.
//sf::Vector2f vect2fSteer(sf::Vector2f velocity, sf::Vector2f desired) {
//    return desired - velocity;
//}

sf::Vector2f boid::align(std::vector<boid*> boidVector) {
    int total = 0;
    sf::Vector2f alignVect = sf::Vector2f(0,0);

    for (boid* entity : boidVector) {
        float distance = vect2fDist(position, entity->getPos());
        if (entity->getID() != id && distance < range*range) {
            alignVect += entity->getVel();
            total += 1;
        }
    }
    if (total > 0) {
        alignVect.x = alignVect.x / total;
        alignVect.y = alignVect.y / total;
        alignVect -= velocity;


        alignVect.x = alignVect.x * alignmentStrength / sqrtf(alignVect.x * alignVect.x + alignVect.y * alignVect.y);
        alignVect.y = alignVect.y * alignmentStrength / sqrtf(alignVect.x * alignVect.x + alignVect.y * alignVect.y);

    }
    return alignVect;
}

sf::Vector2f boid::cohesion(std::vector<boid*> boidVector) {
    int total = 0;
    sf::Vector2f cohesionVect = sf::Vector2f(0, 0);

    for (boid* entity : boidVector) {
        float distance = vect2fDist(position, entity->getPos());
        if (entity->getID() != id && distance < range * range) {
            cohesionVect += entity->getPos();
            total += 1;
        }
    }
    if (total > 0) {
        cohesionVect.x = cohesionVect.x / total;
        cohesionVect.y = cohesionVect.y / total;
        cohesionVect -= position;

        cohesionVect.x = cohesionVect.x * cohesionStrength / sqrtf(cohesionVect.x * cohesionVect.x + cohesionVect.y * cohesionVect.y);
        cohesionVect.y = cohesionVect.y * cohesionStrength / sqrtf(cohesionVect.x * cohesionVect.x + cohesionVect.y * cohesionVect.y);


    }
    return cohesionVect;
}

sf::Vector2f boid::seperation(std::vector<boid*> boidVector) {
    int total = 0;
    sf::Vector2f seperationVect = sf::Vector2f(0, 0);

    for (boid* entity : boidVector) {
        float distance = vect2fDist(position, entity->getPos());
        if (entity->getID() != id && distance < range * range) {
            sf::Vector2f difference = position - entity->getPos();
            difference.x *= 1 / distance;
            difference.y *= 1 / distance;


            seperationVect += difference;
            total += 1;
        }
    }
    if (total > 0) {
        seperationVect.x = seperationVect.x / total;
        seperationVect.y = seperationVect.y / total;

        seperationVect.x = seperationVect.x * seperationStrength / sqrtf(seperationVect.x * seperationVect.x + seperationVect.y * seperationVect.y);
        seperationVect.y = seperationVect.y * seperationStrength / sqrtf(seperationVect.x * seperationVect.x + seperationVect.y * seperationVect.y);

        
    }
    return seperationVect;
}

void boid::flock(std::vector<boid*> boidVector) {
    sf::Vector2f alignment = this->align(boidVector);
    sf::Vector2f cohesion = this->cohesion(boidVector);
    sf::Vector2f seperation = this->seperation(boidVector);
    acceleration = seperation + alignment + cohesion;
    // Limit Acceleration
    if (acceleration.x > maxAcceleration) {
        acceleration.x = maxAcceleration;
    } else if (acceleration.x < -maxAcceleration) {
        acceleration.x = -maxAcceleration;
    }
    if (acceleration.y > maxAcceleration) {
        acceleration.y = maxAcceleration;
    } else if (acceleration.y < -maxAcceleration) {
        acceleration.y = -maxAcceleration;
    }

}

void boid::boidBehavior(std::vector<boid*> boidVector) {
    /*
    * Contains align, cohesion, and seperation all in one function. This allows for some optimization as it only has to loop over neighbors once.
    */
    int Total = 0;

    sf::Vector2f cohesionVect = sf::Vector2f(0, 0);
    sf::Vector2f alignVect = sf::Vector2f(0, 0);
    sf::Vector2f seperationVect = sf::Vector2f(0, 0);

    for (boid* entity : boidVector) {
        float distance = vect2fDist(position, entity->getPos());
        if (entity->getID() != id && distance < range * range) {
            alignVect += entity->getVel();

            cohesionVect += entity->getPos();

            sf::Vector2f difference = position - entity->getPos();
            difference.x *= 1 / distance;
            difference.y *= 1 / distance;

            seperationVect += difference;
            Total += 1;
        }
    }

    if (Total > 0) {
        cohesionVect.x = cohesionVect.x / Total;
        cohesionVect.y = cohesionVect.y / Total;
        cohesionVect -= position;

        float multipleCohesion = cohesionStrength / sqrtf(cohesionVect.x * cohesionVect.x + cohesionVect.y * cohesionVect.y);

        cohesionVect.x = cohesionVect.x * multipleCohesion;
        cohesionVect.y = cohesionVect.y * multipleCohesion;



        alignVect.x = alignVect.x / Total;
        alignVect.y = alignVect.y / Total;
        alignVect -= velocity;

        float multipleAlign = alignmentStrength / sqrtf(alignVect.x * alignVect.x + alignVect.y * alignVect.y);

        alignVect.x = alignVect.x * multipleAlign;
        alignVect.y = alignVect.y * multipleAlign;



        seperationVect.x = seperationVect.x / Total;
        seperationVect.y = seperationVect.y / Total;
        
        float multipleSeperation = seperationStrength / sqrtf(seperationVect.x * seperationVect.x + seperationVect.y * seperationVect.y);

        seperationVect.x = seperationVect.x * multipleSeperation;
        seperationVect.y = seperationVect.y * multipleSeperation;


    }

    
    acceleration = seperationVect + alignVect + cohesionVect;
    // Limit Acceleration
    if (acceleration.x > maxAcceleration) {
        acceleration.x = maxAcceleration;
    } else if (acceleration.x < -maxAcceleration) {
        acceleration.x = -maxAcceleration;
    }
    if (acceleration.y > maxAcceleration) {
        acceleration.y = maxAcceleration;
    } else if (acceleration.y < -maxAcceleration) {
        acceleration.y = -maxAcceleration;
    }
}

void boid::update(std::vector<boid*> boidVector) {
    this->boidBehavior(boidVector);

    velocity += acceleration;
    // Cap Velocity
    velocity.x = velocity.x * maxSpeed / sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
    velocity.y = velocity.y * maxSpeed / sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
    // Add position to Velocity
    position += velocity;

    

    // Screen Wrapping
    if (position.x > screenWidth) {
        position.x = 0;
    } else if (position.x < 0) {
        position.x = screenWidth;
    }
    if (position.y > screenHeight) {
        position.y = 0;
    } else if (position.y < 0) {
        position.y = screenHeight;
    }
}

void boid::render(sf::RenderWindow& window, sf::CircleShape& circle) {
    circle.setPosition(position);
    window.draw(circle);
}