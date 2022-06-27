#include "pch.h"
#include "boid.h"


// Screen Size
extern const int screenWidth = 1600;
extern const int screenHeight = 800;

void update(std::vector<boid*> boidVector) {
    for (boid* entity : boidVector) {
        entity->update(boidVector);
    }
}

void render(sf::RenderWindow &window, std::vector<boid*> boidVector) {
    // Clear Window
    window.clear(sf::Color::Black);
    sf::CircleShape circle(5);

    // Render Boids
    for (boid* entity : boidVector) {
        entity->render(window, circle);
    }
    window.display();
}

int main() {
    srand(time(NULL));
    // Create the window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "My window");

    std::vector<boid*> boids;

    // Create Boids Randomly
    int maxSpeed = 4;
    for (int i = 0; i < 1500; i++) {
        int x = 0;
        while (x == 0) {
            x = rand() % maxSpeed*2+1 - maxSpeed;
        }
        int y = 0;
        while (y == 0) {
            y = rand() % maxSpeed * 2 + 1 - maxSpeed;
        }
        boids.push_back(new boid(rand() % (screenWidth-5), rand() % (screenHeight-5), x, y, boids.size()));
    }

    sf::Clock clock;
    float lastTime = 0;

    //window.setFramerateLimit(60); // Left in for Debugging Purposes
    
    // Update Loop
    while (window.isOpen()) {
        // FPS Counter
        float currentTime = clock.getElapsedTime().asSeconds();
        float fps = 1.f / (currentTime - lastTime);
        lastTime = currentTime;

        std::cout << fps << std::endl;

        // Check all Events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window when requested.
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update Boids
        update(boids);

        // Render the Window and its contents
        render(window, boids);
    }

    return 0;
}
