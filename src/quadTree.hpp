#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "ball.hpp"

// Define a structure to represent a rectangle
struct Rectangle {
    sf::Vector2<float> topLeft;
    float width, height;
};

class QuadTree {
public:
    Rectangle boundary;
    std::vector<Ball> balls;
    std::vector<QuadTree> children;
    bool divided;

    // Constructor
    QuadTree(const Rectangle& boundary) : boundary(boundary), divided(false) {}

    // Subdivide the node into four child nodes
    void subdivide();

    // Insert a ball into the quad tree
    void insert(const Ball& ball);

    // Query nearby balls within a given range
    std::vector<Ball> queryRange(const Ball& range);

private:
    // Check if a ball is inside the node's boundary
    bool contains(const Ball& ball);

    // Helper function for querying nearby balls recursively
    void queryRangeHelper(const Ball& range, std::vector<Ball>& foundBalls);

    // Check if the node's boundary intersects with a given range
    bool boundaryIntersects(const Ball& range);

    // Check if a ball intersects with a given range
    bool ballIntersectsRange(const Ball& ball, const Ball& range);
};

#endif