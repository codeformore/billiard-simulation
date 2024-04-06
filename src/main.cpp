#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include "ball.hpp"

const int NUM_BALLS = 50;

int main()
{
    //Initialize Random Number Generator
    std::random_device rd;
    std::mt19937 gen(rd());
    //Define Distributions
    std::uniform_real_distribution<float> rand_mass(1.0f, 5.0f);
    std::uniform_real_distribution<float> rand_radius(5.0f, 15.0f); 
    std::uniform_real_distribution<float> rand_position(200.0f, 500.0f);
    std::uniform_real_distribution<float> rand_velocity(-5.0f, 5.0f);

    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Billiard Simulation");

    sf::Clock clock;

    std::vector<Ball> balls;
    for (int i = 0; i < NUM_BALLS; i++)
    {
        balls.emplace_back(rand_radius(gen), rand_mass(gen));
        balls[i].position.x = rand_position(gen); balls[i].position.y = rand_position(gen);
        balls[i].velocity.x = rand_velocity(gen); balls[i].velocity.y = rand_velocity(gen);
    }

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaT = clock.restart().asSeconds();

        //Move Shape
        for (auto& ball : balls)
            ball.update(deltaT);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        for (auto& ball : balls)
            window.draw(ball.shape);

        // end the current frame
        window.display();
    }

    return 0;
}