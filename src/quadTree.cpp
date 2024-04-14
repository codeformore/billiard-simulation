#include "quadTree.hpp"
#include <iostream>
#include <cmath>

// Subdivide the node into four child nodes
void QuadTree::subdivide() {
    float x = Boundary.topLeft.x;
    float y = Boundary.topLeft.y;
    float w = Boundary.width / 2;
    float h = Boundary.height / 2;

    Children.emplace_back(Rectangle{sf::Vector2<float>(x + w, y), w, h}); // Top-right
    Children.emplace_back(Rectangle{sf::Vector2<float>(x, y), w, h});     // Top-left
    Children.emplace_back(Rectangle{sf::Vector2<float>(x, y + h), w, h}); // Bottom-left
    Children.emplace_back(Rectangle{sf::Vector2<float>(x + w, y + h), w, h}); // Bottom-right

    Divided = true;
}

// Insert a ball into the quad tree
void QuadTree::Insert(sf::Vector2<float> position, float radius, int ball_num) {
    if (!contains(BallCollisionBox{position, radius, ball_num})) {
        return; // Ball is outside the node's boundary
    }

    if (!Divided && Balls.size() < 4) {
        Balls.push_back(BallCollisionBox{position, radius, ball_num}); // Add ball to the node
    } else {
        if (!Divided) {
            subdivide(); // Subdivide the node if not already divided
            for (int i = 0; i < 4; i++)
                for (const auto& ball : Balls)
                    Children[i].Insert(ball.center, ball.radius, ball.ball_num);
            Balls.clear();
        }
        // Recursively insert the ball into the appropriate child node
        for (int i = 0; i < 4; i++) {
            Children[i].Insert(position, radius, ball_num);
        }
    }
}

// Query nearby balls within a given range
std::vector<BallCollisionBox> QuadTree::QueryRange(const BallCollisionBox& range) {
    std::vector<BallCollisionBox> foundBalls;
    queryRangeHelper(range, foundBalls);
    return foundBalls;
}

void QuadTree::Clear()
{
    Balls.clear(); // Clear the vector of balls
    for (auto& child : Children) {
        child.Clear(); // Recursively clear child nodes
    }
    Children.clear();
    Divided = false;
}

void QuadTree::Draw(sf::RenderWindow &window)
{
    for (auto& vBdd : VBoundaries)
        window.draw(vBdd);
    for (auto& child : Children)
        child.Draw(window);
}

bool QuadTree::contains(const BallCollisionBox& ball) {
    return (ball.center.x + ball.radius >= Boundary.topLeft.x                         //Left side collision
            && ball.center.x - ball.radius <= Boundary.topLeft.x + Boundary.width     //Right side collision
            && ball.center.y + ball.radius >= Boundary.topLeft.y                      //Top side collision
            && ball.center.y - ball.radius <= Boundary.topLeft.y + Boundary.height);  //Bottom side collision
}

// Helper function for querying nearby balls recursively
void QuadTree::queryRangeHelper(const BallCollisionBox& range, std::vector<BallCollisionBox>& foundBalls) {
    if (!boundaryIntersects(range)) {
        return;
    }
    for (const auto& ball : Balls) {
        if (ballIntersectsRange(ball, range)) {
            foundBalls.push_back(ball);
        }
    }
    if (Divided) {
        for (int i = 0; i < 4; ++i) {
            Children[i].queryRangeHelper(range, foundBalls);
        }
    }
}

// Check if the node's boundary intersects with a given range
bool QuadTree::boundaryIntersects(const BallCollisionBox& range) {
    Rectangle rangeRect = {sf::Vector2<float>{range.center.x - range.radius, range.center.y - range.radius}, 2 * range.radius, 2 * range.radius};
    return !(Boundary.topLeft.x > rangeRect.topLeft.x + rangeRect.width ||
                Boundary.topLeft.x + Boundary.width < rangeRect.topLeft.x ||
                Boundary.topLeft.y > rangeRect.topLeft.y + rangeRect.height ||
                Boundary.topLeft.y + Boundary.height < rangeRect.topLeft.y);
}

// Check if a ball intersects with a given range
bool QuadTree::ballIntersectsRange(const BallCollisionBox& ball, const BallCollisionBox& range) {
    // Check if the distance between the centers of the two balls is less than the sum of their radii
    float dx = ball.center.x - range.center.x;
    float dy = ball.center.y - range.center.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSumSquared = (ball.radius + range.radius) * (ball.radius + range.radius);
    return distanceSquared <= radiusSumSquared;
}