#include <SFML/Graphics.hpp>
#include <iostream>

//Function Prototypes
void applyGravity(sf::Vector2f&);

int main() {
    //Create window and set frame rate
    sf::RenderWindow window(sf::VideoMode(1000,1000), "SFML Window");
    const int frameCap = 60;
    window.setFramerateLimit(frameCap);

    //Create object for testing
    float radius = 50.f;
    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color(255,0,0));
    sf::Vector2f circlePosition(500-radius, 500-radius);
    shape.setPosition(circlePosition);

    sf::Vector2f position = circlePosition;
    sf::Vector2f lastPosition = circlePosition;
    lastPosition.y -= 50;
    sf::Vector2f acceleration;
    sf::Clock clock;

    while (window.isOpen()){
        //Event list for keyboard commands
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Escape) window.close();
                if(event.key.code == sf::Keyboard::R) {
                    acceleration.x -= 500;
                    acceleration.y -= 500;
                }
                if(event.key.code == sf::Keyboard::Space) acceleration.x += 2000;
            }

        }


        //Calculating position after applying gravitational acceleration
        float dt = clock.restart().asSeconds();
        applyGravity(acceleration);


        sf::Vector2f displacement = position - lastPosition;
        if(position.y < 0 || position.y > 1000 - radius*2) {
            acceleration.y *= -1;
            if(position.y > 1000-radius*2) position.y = 1000-radius*2;
        }
        if(position.x < 0 || position.x > 1000 - radius*2) {
            acceleration.x *= -1;
        }
        lastPosition = position;
        position += displacement + acceleration*dt*dt;

        std::cout << "Position: " << position.y << std::endl;
        std::cout << "Displacement: " << displacement.y << std::endl;
        std::cout << "Acceleration in Y: " << acceleration.y << std::endl;
        std::cout << dt << std::endl;

        //Reset values
        acceleration = {};

        //Updating and drawing shape in new location
        shape.setPosition(position);
        window.clear();
        window.draw(shape);
        window.display();

    }

    return 0;
}

void applyGravity(sf::Vector2f& acceleration){
    float pixelsPerMeter = 400.f;
    acceleration.y += 9.81 * pixelsPerMeter;
}