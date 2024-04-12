#include "quadTree.hpp"
#include <iostream>
#include <cmath>

// Subdivide the node into four child nodes
void QuadTree::subdivide() {
    float x = boundary.topLeft.x;
    float y = boundary.topLeft.y;
    float w = boundary.width / 2;
    float h = boundary.height / 2;

    children.emplace_back(Rectangle{sf::Vector2<float>(x + w, y), w, h}); // Top-right
    children.emplace_back(Rectangle{sf::Vector2<float>(x, y), w, h});     // Top-left
    children.emplace_back(Rectangle{sf::Vector2<float>(x, y + h), w, h}); // Bottom-left
    children.emplace_back(Rectangle{sf::Vector2<float>(x + w, y + h), w, h}); // Bottom-right

    divided = true;
}

// Insert a ball into the quad tree
void QuadTree::insert(const Ball& ball) {
    if (!contains(ball)) {
        return; // Ball is outside the node's boundary
    }

    if (balls.size() < 4 || divided) {
        balls.push_back(ball); // Add ball to the node
    } else {
        if (!divided) {
            subdivide(); // Subdivide the node if not already divided
        }
        // Recursively insert the ball into the appropriate child node
        for (int i = 0; i < 4; ++i) {
            children[i].insert(ball);
        }
    }
}

// Query nearby balls within a given range
std::vector<Ball> QuadTree::queryRange(const Ball& range) {
    std::vector<Ball> foundBalls;
    queryRangeHelper(range, foundBalls);
    return foundBalls;
}


bool QuadTree::contains(const Ball& ball) {
    return (ball.position.x >= boundary.topLeft.x && ball.position.x <= boundary.topLeft.x + boundary.width &&
            ball.position.y >= boundary.topLeft.y && ball.position.y <= boundary.topLeft.y + boundary.height);
}

// Helper function for querying nearby balls recursively
void QuadTree::queryRangeHelper(const Ball& range, std::vector<Ball>& foundBalls) {
    if (!boundaryIntersects(range)) {
        return;
    }
    for (const auto& ball : balls) {
        if (ballIntersectsRange(ball, range)) {
            foundBalls.push_back(ball);
        }
    }
    if (divided) {
        for (int i = 0; i < 4; ++i) {
            children[i].queryRangeHelper(range, foundBalls);
        }
    }
}

// Check if the node's boundary intersects with a given range
bool QuadTree::boundaryIntersects(const Ball& range) {
    Rectangle rangeRect = {sf::Vector2<float>{range.position.x - range.radius, range.position.y - range.radius}, 2 * range.radius, 2 * range.radius};
    return !(boundary.topLeft.x > rangeRect.topLeft.x + rangeRect.width ||
                boundary.topLeft.x + boundary.width < rangeRect.topLeft.x ||
                boundary.topLeft.y > rangeRect.topLeft.y + rangeRect.height ||
                boundary.topLeft.y + boundary.height < rangeRect.topLeft.y);
}

// Check if a ball intersects with a given range
bool QuadTree::ballIntersectsRange(const Ball& ball, const Ball& range) {
    // Check if the distance between the positions of the two balls is less than the sum of their radii
    float dx = ball.position.x - range.position.x;
    float dy = ball.position.y - range.position.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSumSquared = (ball.radius + range.radius) * (ball.radius + range.radius);
    return distanceSquared <= radiusSumSquared;
}

// int main() {
//     QuadTreeNode quadTree(Rectangle{Point{0, 0}, 800, 600}); // Initialize the quad tree with the boundary of the 2D space

//     // Insert balls into the quad tree
//     quadTree.insert(Circle{Point{100, 100}, 20});
//     quadTree.insert(Circle{Point{200, 200}, 25});
//     quadTree.insert(Circle{Point{300, 300}, 30});

//     // Query nearby balls within a given range
//     Circle range = {Point{150, 150}, 50};
//     std::vector<Circle> nearbyBalls = quadTree.queryRange(range);

//     // Output the nearby balls
//     std::cout << "Nearby balls:" << std::endl;
//     for (const auto& ball : nearbyBalls) {
//         std::cout << "Center: (" << ball.position.x << ", " << ball.position.y << "), Radius: " << ball.radius << std::endl;
//     }

//     return 0;
// }