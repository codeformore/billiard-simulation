#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include "ball.hpp"
#include "quadTree.hpp"

const int NUM_BALLS = 250;
const int MAX_NUM_TRIES = 5;

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

    //NEW VECTOR METHOD
    Rectangle boundary = {sf::Vector2<float>(0.0f, 0.0f), 800, 800};
    QuadTree qTree(boundary);
    std::vector<Ball> balls;
    bool found_free_space = true;
    for (int i = 0; i < NUM_BALLS && found_free_space; i++)
    {
        int tries = 0;
        sf::Vector2<float> position_candidate;
        float radius_candidate;
        found_free_space = false;
        while (tries < MAX_NUM_TRIES && !found_free_space)
        {
            position_candidate = sf::Vector2(rand_position(gen) + 200, rand_position(gen) + 200);
            radius_candidate = rand_radius(gen);
            Ball test_ball = Ball(position_candidate, sf::Vector2(0.0f, 0.0f), sf::Vector2(0.0f, 0.0f), radius_candidate, 1.0f);
            found_free_space = true;
            for (const auto& ball : balls)
                found_free_space &= !Ball::AreColliding(ball, test_ball);
            if (found_free_space)
                balls.emplace_back(position_candidate,
                                sf::Vector2(rand_velocity(gen), rand_velocity(gen)),
                                sf::Vector2(0.0f, 0.0f),
                                radius_candidate, rand_mass(gen));
            tries++;
        }
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
        //SLOW: Check all pairs
        // for (int i = 1; i < balls.size(); i++)
        // {
        //     for (int j = 0; j < i; j++)
        //     {
        //         if (i != j)
        //         {
        //             if (Ball::AreColliding(balls[i], balls[j]))
        //             {
        //                 balls[i].shape.setFillColor(sf::Color::Red);
        //                 balls[j].shape.setFillColor(sf::Color::Red);
        //                 Ball::CalculateElasticCollision(balls[i], balls[j]);
        //                 // balls[i].update(deltaT); balls[j].update(deltaT);
        //             }
        //         }
        //     }
        // }

        //FAST: Quadtree
        //Update Quad Tree
        qTree.Clear();
        for (int i = 0; i < balls.size(); i++)
            qTree.Insert(balls[i].position, balls[i].radius, i);
        //Perform Collision Detection
        for (int i = 0; i < balls.size(); i++) {
            // Query nearby balls within a certain range
            std::vector<int> nearbyBalls = qTree.QueryRange(balls[i].position, balls[i].radius, i);

            // Calculate collisions for found balls
            for (const auto& nearbyBall : nearbyBalls) {
                if (nearbyBall > i)
                {
                    balls[i].shape.setFillColor(sf::Color::Red);
                    balls[nearbyBall].shape.setFillColor(sf::Color::Red);
                    sf::Vector2<float> prev_momentum = balls[i].velocity*balls[i].mass + balls[nearbyBall].velocity*balls[nearbyBall].mass;
                    Ball::CalculateElasticCollision(balls[i], balls[nearbyBall]);
                    //Simple Sticking Fix. Not physically accurate.
                    while (Ball::AreColliding(balls[i], balls[nearbyBall]))
                    {
                        balls[i].update(deltaT); 
                        balls[nearbyBall].update(deltaT);
                    }
                    sf::Vector2<float> after_momentum = balls[i].velocity*balls[i].mass + balls[nearbyBall].velocity*balls[nearbyBall].mass;
                    sf::Vector2<float> diff_momentum = after_momentum - prev_momentum;
                    float diff_momentum_norm_squared = diff_momentum.x*diff_momentum.x + diff_momentum.y*diff_momentum.y;
                    if (diff_momentum_norm_squared > 1e-5)
                        std::cout << "ERROR: Momentum Not Conserved: " << diff_momentum_norm_squared << std::endl;
                }
            }
        }

        //Keep inside of screen collision detection
        for (auto& ball : balls)
        {
            if (ball.position.x - ball.radius < boundary.topLeft.x || ball.position.x + ball.radius > boundary.topLeft.x + boundary.width)
                ball.velocity.x = -ball.velocity.x; 
            if (ball.position.y - ball.radius < boundary.topLeft.y || ball.position.y + ball.radius > boundary.topLeft.y + boundary.height)
                ball.velocity.y = -ball.velocity.y;
        }

        //Calculate Physics Updates
        for (auto& ball : balls)
            ball.update(deltaT);
        
        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        qTree.Draw(window);
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