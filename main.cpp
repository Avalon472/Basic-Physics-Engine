#include <SFML/Graphics.hpp>
#include <iostream>

//to compile program: g++ main.cpp -0 engine -lsfml-graphics -lsfml-window -lsfml-system
class ball{
    private:
    float radius;
    sf::Vector2f circlePosition;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Color ballColor;

    public:
    sf::CircleShape self;
    //Constructors
    ball(){
    radius = 50.f;
    circlePosition.x = 500-radius;
    circlePosition.y = 500-radius;
    position = circlePosition;
    self.setPosition(position);
    self.setFillColor(ballColor);
    }
    ball(float velX, float velY):ball(){
        velocity.x = velX;
        velocity.y = velY;

    }
    ball(sf::Vector2f startPoint, sf::Color hue, float size):ball(){
        ballColor = hue;
        radius = size;
        circlePosition = startPoint;
        self.setPosition(circlePosition);
        self.setFillColor(ballColor);
        self.setRadius(radius);
    }

    //A few getter and setter functions
    sf::Vector2f getPosition() {
        return self.getPosition();
    }
    sf::Vector2f getVelocity(){
        return velocity;
    }
    void setVelocity(sf::Vector2f newVel){
        velocity = newVel;
    }

    //For checking if the ball has hit a wall
    sf::Vector2f boundsDetection(sf::Vector2f currentPos){
        sf::Vector2f newPos = currentPos;
        if(currentPos.y<0){
            newPos.y = 0;
            velocity.y *= -0.9;
        }
        //Bottom wall detection
        if(currentPos.y> 1000 - radius*2){
            newPos.y = 1000 - radius*2;
            velocity.y *= -0.9;
        }
        //Left wall detection
            if(currentPos.x<0){
            newPos.x = 0;
            velocity.x *= -0.8;
        }
        //Right wall detection
        if(currentPos.x> 1000 - radius*2){
            newPos.x = 1000 - radius*2;
            velocity.x *= -0.8;
        }
        return newPos;
    }

    void applyForce(sf::Vector2f force){
        acceleration += force;
    }

    void simulate(float dt){
        sf::Vector2f gravity = {0, 9.81*400};
        applyForce(gravity);
        velocity += acceleration*dt;
        sf::Vector2f newPosition = position + velocity*dt;
        newPosition = boundsDetection(newPosition);
        position = newPosition;
        acceleration = {};
        self.setPosition(position);
    }

};


int main() {
    //Create window and set frame rate
    sf::RenderWindow window(sf::VideoMode(1000,1000), "SFML Window");
    const int frameCap = 60;
    window.setFramerateLimit(frameCap);

    //Create object for testing
    float radius = 50.f;
    sf::Vector2f circlePosition(500-radius, 500-radius);
    ball firstBall(circlePosition, sf::Color(255,0,0), radius);


    sf::Vector2f position = circlePosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Clock clock;

    while (window.isOpen()){
        //Event list for keyboard commands
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
                    sf::Vector2f direction = mousePos - firstBall.getPosition();
                    firstBall.setVelocity(firstBall.getVelocity()*0.5f);
                    firstBall.applyForce(direction*1500.f);
                }
            }
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Escape) window.close();
                if(event.key.code == sf::Keyboard::A) {
                    firstBall.applyForce({-10000, 500});
                }
                if(event.key.code == sf::Keyboard::D) {
                    firstBall.applyForce({10000, 500});
                }
            }

        }

        //Calculating position after applying gravitational acceleration
        float dt = clock.restart().asSeconds();
        int steps = 6;
        float sub_dt = dt/steps;
        for(int i = 0; i<steps; i++)
            firstBall.simulate(sub_dt);

        window.clear();
        window.draw(firstBall.self);
        window.display();

    }

    return 0;
}
