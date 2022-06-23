#include "pch.h"
#include "boid.h"


// Screen Size
extern const int screenWidth = 800;
extern const int screenHeight = 800;
// Create the grid everything takes place on.
const int xCells = 40;
const int yCells = 40;
// Size of the cells
const int xCellSize = screenWidth / xCells;
const int yCellSize = screenHeight / yCells;

void initGrid(sf::VertexArray& grid) {
    int x_space = screenWidth / xCells;
    int y_space = screenHeight / yCells;

    for (int x = 1; x < xCells; x++) {
        // Vertical Lines
        grid.append(sf::Vector2f(x * x_space, 0));
        grid.append(sf::Vector2f(x * x_space, screenHeight));
        for (int y = 1; y < yCells; y++) {
            // Horizontal Lines
            grid.append(sf::Vector2f(0, y * y_space));
            grid.append(sf::Vector2f(screenWidth, y * y_space));
        }
    }
}

void update(std::vector<boid*> boidVector) {
    for (boid* entity : boidVector) {
        entity->update(boidVector);
    }
}

void render(sf::RenderWindow &window,sf::VertexArray grid, std::vector<boid*> boidVector) {

    // Clear Window
    window.clear(sf::Color::Black);
    // Render Boids


    for (boid* entity : boidVector) {
        entity->render(window);

    }
    // Draw Grid
    //window.draw(grid);
    // Display Window
    window.display();
}

int main() {
    srand(time(NULL));
    // Create the window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "My window");

    // Displayed Grid
    sf::VertexArray grid(sf::Lines);
    initGrid(grid);

    std::vector<boid*> boids;
    ///*
    int maxSpeed = 4;
    for (int i = 0; i < 1000; i++) {
        int x = 0;
        while (x == 0) {
            x = rand() % maxSpeed*2+1 - maxSpeed;
        }
        int y = 0;
        while (y == 0) {
            y = rand() % maxSpeed * 2 + 1 - maxSpeed;
        }
        boids.push_back(new boid(rand() % (screenWidth-5), rand() % (screenHeight-5), x, y, boids.size()));
        //boids.push_back(new boid(rand() % (screenWidth - 5), rand() % (screenHeight - 5), 0, 0.5, boids.size()));
    }

    window.setFramerateLimit(60);
    // Update Loop
    while (window.isOpen()) {

        // Check all Events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window when requested.
            if (event.type == sf::Event::Closed)
                window.close();
        }



        update(boids);


        // Render the Window and its contents
        render(window, grid, boids);
    }

    return 0;
}
