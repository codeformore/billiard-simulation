#include "ball.hpp"

void Ball::update(float deltaT)
{
    position = velocity*deltaT + position;
    shape.setPosition(position - sf::Vector2(radius, radius));
    velocity = acceleration*deltaT + velocity;
}

Ball::Ball(sf::Vector2<float> init_position, 
           sf::Vector2<float> init_velocity, 
           sf::Vector2<float> init_acceleration, 
           float radius, float mass)
{
    this->radius = radius;
    this->mass = mass;
    position = init_position;
    velocity = init_velocity;
    acceleration = init_acceleration;

    shape = sf::CircleShape(radius);
    shape.setFillColor(sf::Color(100,250,50));
    shape.setPosition(position - sf::Vector2(radius, radius));
}

