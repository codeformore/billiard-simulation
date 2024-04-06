#include <SFML/Graphics.hpp>
#include "ball.hpp"

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Billiard Simulation");

    float deltaT = 0.1;
    Ball myBall;
    myBall.velocity.x = 3; myBall.velocity.y = 3;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Move Shape
        myBall.update(0.01);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        window.draw(myBall.shape);

        // end the current frame
        window.display();
    }

    return 0;
}