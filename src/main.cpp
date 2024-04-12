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
    std::uniform_real_distribution<float> rand_mass(1.0f, 20.0f);
    std::uniform_real_distribution<float> rand_radius(5.0f, 10.0f); 
    std::uniform_real_distribution<float> rand_position(0.0f, 400.0f);
    std::uniform_real_distribution<float> rand_velocity(-50.0f, 50.0f);

    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Billiard Simulation");

    sf::Clock clock;

    //Random Balls Test
    std::vector<Ball> balls;
    for (int i = 0; i < NUM_BALLS; i++)
    {
        balls.emplace_back(sf::Vector2(rand_position(gen) + 200, rand_position(gen) + 200),
                           sf::Vector2(rand_velocity(gen), rand_velocity(gen)),
                           sf::Vector2(0.0f, 0.0f),
                           rand_radius(gen), rand_mass(gen));
    }

    //Four Corners Test
    // balls.emplace_back(sf::Vector2(500.0f,500.0f),
    //                    sf::Vector2(-30.0f,-30.0f),
    //                    sf::Vector2(0.0f, 0.0f),
    //                    10.0f, 1.0f);
    // balls.emplace_back(sf::Vector2(300.0f,300.0f),
    //                    sf::Vector2(30.0f,30.0f),
    //                    sf::Vector2(0.0f, 0.0f),
    //                    10.0f, 1.0f);
    // balls.emplace_back(sf::Vector2(500.0f,300.0f),
    //                    sf::Vector2(-30.0f,30.0f),
    //                    sf::Vector2(0.0f, 0.0f),
    //                    10.0f, 1.0f);
    // balls.emplace_back(sf::Vector2(300.0f,500.0f),
    //                    sf::Vector2(30.0f,-30.0f),
    //                    sf::Vector2(0.0f, 0.0f),
    //                    10.0f, 1.0f);

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

        //Determine Collisions
        for (int i = 1; i < balls.size(); i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (i != j)
                {
                    if (Ball::AreColliding(balls[i], balls[j]))
                    {
                        balls[i].shape.setFillColor(sf::Color::Red);
                        balls[j].shape.setFillColor(sf::Color::Red);
                        Ball::CalculateElasticCollision(balls[i], balls[j]);
                        // balls[i].update(deltaT); balls[j].update(deltaT);
                    }
                }
            }
        }

        //Calculate Physics Updates
        for (auto& ball : balls)
            ball.update(deltaT);
        
        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        for (auto& ball : balls)
            window.draw(ball.shape);

        //Reset Colors
        for (auto& ball : balls)
            ball.shape.setFillColor(sf::Color::Blue);

        // end the current frame
        window.display();
    }

    return 0;
}