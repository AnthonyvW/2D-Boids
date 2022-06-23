
#include "pch.h"
#include "boid.h"
#include <iostream>

extern const int screenWidth;
extern const int screenHeight;

float range = 35;
float seperationStrength = 0.8;
float cohesionStrength = 0.3;
float alignmentStrength = 0.5;
float maxAcc = 0.3;

float maxSpeed = 4;

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
    return (pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y);
}

bool isPointInCircle(sf::Vector2f pos1, sf::Vector2f pos2, float r) {
    return (pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y) < (r * r);
}

sf::Vector2f vect2fSteer(sf::Vector2f velocity, sf::Vector2f desired) {
    return desired - velocity;
}

sf::Vector2f boid::align(std::vector<boid*> boidVector) {
    int total = 0;
    sf::Vector2f desiredVect = sf::Vector2f(0,0);

    for (boid* entity : boidVector) {
        float distance = vect2fDist(position, entity->getPos());
        if (entity->getID() != id && distance < range*range) {
            desiredVect += entity->getVel();
            total += 1;
        }
    }
    if (total > 0) {
        desiredVect.x = desiredVect.x / total;
        desiredVect.y = desiredVect.y / total;
        desiredVect -= velocity;


        desiredVect.x = desiredVect.x * alignmentStrength / sqrtf(desiredVect.x * desiredVect.x + desiredVect.y * desiredVect.y);
        desiredVect.y = desiredVect.y * alignmentStrength / sqrtf(desiredVect.x * desiredVect.x + desiredVect.y * desiredVect.y);

    }
    return desiredVect;
}

sf::Vector2f boid::cohesion(std::vector<boid*> boidVector) {
    int total = 0;
    sf::Vector2f desiredVect = sf::Vector2f(0, 0);

    for (boid* entity : boidVector) {
        float distance = vect2fDist(position, entity->getPos());
        if (entity->getID() != id && distance < range * range) {
            desiredVect += entity->getPos();
            total += 1;
        }
    }
    if (total > 0) {
        desiredVect.x = desiredVect.x / total;
        desiredVect.y = desiredVect.y / total;
        desiredVect -= position;

        desiredVect.x = desiredVect.x * cohesionStrength / sqrtf(desiredVect.x * desiredVect.x + desiredVect.y * desiredVect.y);
        desiredVect.y = desiredVect.y * cohesionStrength / sqrtf(desiredVect.x * desiredVect.x + desiredVect.y * desiredVect.y);


    }
    return desiredVect;
}

sf::Vector2f boid::seperation(std::vector<boid*> boidVector) {
    int total = 0;
    sf::Vector2f desiredVect = sf::Vector2f(0, 0);

    for (boid* entity : boidVector) {
        float distance = vect2fDist(position, entity->getPos());
        if (entity->getID() != id && distance < range * range) {
            sf::Vector2f difference = position - entity->getPos();
            difference.x /= distance;
            difference.y /= distance;


            desiredVect += difference;
            total += 1;
        }
    }
    if (total > 0) {
        desiredVect.x = desiredVect.x / total;
        desiredVect.y = desiredVect.y / total;

        desiredVect.x = desiredVect.x * seperationStrength / sqrtf(desiredVect.x * desiredVect.x + desiredVect.y * desiredVect.y);
        desiredVect.y = desiredVect.y * seperationStrength / sqrtf(desiredVect.x * desiredVect.x + desiredVect.y * desiredVect.y);

        
    }
    return desiredVect;
}

void boid::flock(std::vector<boid*> boidVector) {
    sf::Vector2f alignment = this->align(boidVector);
    sf::Vector2f cohesion = this->cohesion(boidVector);
    sf::Vector2f seperation = this->seperation(boidVector);
    acceleration = seperation + alignment + cohesion;
    // Limit Acceleration
    if (acceleration.x > maxAcc) {
        acceleration.x = maxAcc;
    } else if (acceleration.x < -maxAcc) {
        acceleration.x = -maxAcc;
    }
    if (acceleration.y > maxAcc) {
        acceleration.y = maxAcc;
    } else if (acceleration.y < -maxAcc) {
        acceleration.y = -maxAcc;
    }

}

void boid::update(std::vector<boid*> boidVector) {
    this->flock(boidVector);

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

void boid::render(sf::RenderWindow& window) {
    // Initialize Circle
    sf::CircleShape circle(5);
    circle.setPosition(position);
    window.draw(circle);
}