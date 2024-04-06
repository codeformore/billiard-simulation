#include "ball.hpp"

void Ball::update(float deltaT)
{
    position = velocity*deltaT + position;
    shape.setPosition(position);
    velocity = acceleration*deltaT + velocity;
}

Ball::Ball(float radius, float mass)
{
    shape = sf::CircleShape(radius);
    shape.setFillColor(sf::Color(100,250,50));

    this->radius = radius;
    this->mass = mass;
    position = sf::Vector2<float>(0, 0);
    velocity = sf::Vector2<float>(0, 0);
    acceleration = sf::Vector2<float>(0, 0);
}

