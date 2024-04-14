#include "ball.hpp"
#include <math.h>
//Debug
#include <iostream>

void Ball::update(float deltaT)
{
    position = velocity*deltaT + position;
    shape.setPosition(position - sf::Vector2(radius, radius));
    // velocity = acceleration*deltaT + velocity;
}

bool Ball::AreColliding(const Ball &ball1, const Ball &ball2)
{
    // Calculate distance between circle centers
    sf::Vector2<float> delta = ball1.position - ball2.position;
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    // Check if circles are colliding
    return distance < (ball1.radius + ball2.radius);
}

void Ball::CalculateElasticCollision(Ball& ball1, Ball& ball2)
{
    sf::Vector2<float> velocity_difference = ball1.velocity - ball2.velocity;
    sf::Vector2<float> position_difference = ball1.position - ball2.position;
    float difference_dot = velocity_difference.x*position_difference.x + velocity_difference.y*position_difference.y;
    float difference_norm_squared = position_difference.x*position_difference.x + position_difference.y*position_difference.y;
    float mass_term = 2 / (ball1.mass + ball2.mass);
    ball1.velocity -= (ball2.mass * mass_term * difference_dot / difference_norm_squared) * position_difference;
    ball2.velocity += (ball1.mass * mass_term * difference_dot / difference_norm_squared) * position_difference;
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

