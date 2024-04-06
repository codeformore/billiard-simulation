#include "ball.hpp"
#include <math.h>

void Ball::update(float deltaT)
{
    position = velocity*deltaT + position;
    shape.setPosition(position - sf::Vector2(radius, radius));
    velocity = acceleration*deltaT + velocity;
}

bool Ball::AreColliding(const Ball &ball1, const Ball &ball2)
{
    // Calculate distance between circle centers
    sf::Vector2<float> delta = ball1.position - ball2.position;
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    // Check if circles are colliding
    return distance < (ball1.radius + ball2.radius);
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
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(position - sf::Vector2(radius, radius));
}

