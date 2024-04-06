#ifndef BALL_HPP
#define BALL_HPP
#include <SFML/Graphics.hpp>

class Ball
{
public:

    //Rendering Properties
    sf::CircleShape shape;

    //Physical Properties
    sf::Vector2<float> position;
    sf::Vector2<float> velocity;
    sf::Vector2<float> acceleration;
    float mass;
    float radius;

    //Physical Functions
    void update(float deltaT);

    //Static Collision Functions
    static bool AreColliding(const Ball& ball1, const Ball& ball2);

    Ball(sf::Vector2<float> init_position = sf::Vector2(0.0f, 0.0f), 
         sf::Vector2<float> init_velocity = sf::Vector2(0.0f, 0.0f), 
         sf::Vector2<float> init_acceleration = sf::Vector2(0.0f, 0.0f), 
         float radius = 10, float mass = 1);
};
#endif