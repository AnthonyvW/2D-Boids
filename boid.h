#pragma once
#include "pch.h"
class boid {
private:
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	float max;
	int id;
public:
	boid();
	boid(float newx, float newy, float xDir, float yDir, int id);

	sf::Vector2f getVel();
	sf::Vector2f getPos();
	sf::Vector2f getAcc();

	int getID();
	void flock(std::vector<boid*> boidVector);
	void boidBehavior(std::vector<boid*> boidVector);
	sf::Vector2f align(std::vector<boid*> boidVector);
	sf::Vector2f cohesion(std::vector<boid*> boidVector);
	sf::Vector2f seperation(std::vector<boid*> boidVector);
	void update(std::vector<boid*> boidVector);
	void render(sf::RenderWindow& window, sf::CircleShape& circle);
};

