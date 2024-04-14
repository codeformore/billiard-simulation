#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <SFML/Graphics.hpp>
#include <vector>

// Define a structure to represent a rectangle
struct Rectangle {
    sf::Vector2<float> topLeft;
    float width, height;
};

struct BallCollisionBox {
    sf::Vector2<float> center;
    float radius;
    int ball_num;
};

class QuadTree {
public:
    Rectangle Boundary;
    sf::RectangleShape VBoundaries[4];
    std::vector<BallCollisionBox> Balls;
    std::vector<QuadTree> Children;
    bool Divided;

    // Constructor
    QuadTree(const Rectangle& boundary) : Boundary(boundary), Divided(false) {
        VBoundaries[0] = sf::RectangleShape(sf::Vector2<float>(boundary.width, 3.0f));
        VBoundaries[0].setPosition(boundary.topLeft);
        VBoundaries[1] = sf::RectangleShape(sf::Vector2<float>(boundary.width, 3.0f));
        VBoundaries[1].setPosition(boundary.topLeft + sf::Vector2<float>(0.0f, boundary.height - 3.0f));
        VBoundaries[2] = sf::RectangleShape(sf::Vector2<float>(3.0f, boundary.height));
        VBoundaries[2].setPosition(boundary.topLeft);
        VBoundaries[3] = sf::RectangleShape(sf::Vector2<float>(3.0f, boundary.height));
        VBoundaries[3].setPosition(boundary.topLeft + sf::Vector2<float>(boundary.width - 3.0f, 0.0f));
    }

    // Insert a ball into the quad tree
    void Insert(sf::Vector2<float> position, float radius, int ball_num);

    // Query nearby balls within a given range
    std::vector<BallCollisionBox> QueryRange(const BallCollisionBox& range);

    //Clear the QuadTree
    void Clear();

    //Draw the QuadTree
    void Draw(sf::RenderWindow& window);

private:
    // Subdivide the node into four child nodes
    void subdivide();

    // Check if a ball is inside the node's boundary
    bool contains(const BallCollisionBox& ball);

    // Helper function for querying nearby balls recursively
    void queryRangeHelper(const BallCollisionBox& range, std::vector<BallCollisionBox>& foundBalls);

    // Check if the node's boundary intersects with a given range
    bool boundaryIntersects(const BallCollisionBox& range);

    // Check if a ball intersects with a given range
    bool ballIntersectsRange(const BallCollisionBox& ball, const BallCollisionBox& range);
};

#endif