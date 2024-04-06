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

    Ball(float radius = 10, float mass = 1);
};
#endif