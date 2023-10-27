#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

#define PI 3.14159

class Pendulum {
    sf::Vector2f piv;        // Pivot point
    sf::Vector2f bob;        // First bob
    sf::Vector2f bob2;       // Second bob
    sf::RectangleShape pivot; // Visual representation of pivot
    sf::CircleShape circle;   // Visual representation of the first bob
    sf::CircleShape circle2;  // Visual representation of the second bob
    sf::Vertex line1[2];     // Line connecting pivot to the first bob
    sf::Vertex line2[2];     // Line connecting the first bob to the second bob
    std::vector<sf::Vector2f> path; // Store path points
    sf::VertexArray pathLine;      // Path line
    float windowWidth;      // Width of the window
    float windowHeight;     // Height of the window
    float angle;            // Angle of the first bob
    float angle2;           // Angle of the second bob
    float angleVel;         // Angular velocity of the first bob
    float angleVel2;        // Angular velocity of the second bob
    float angleAcc;         // Angular acceleration of the first bob
    float angleAcc2;        // Angular acceleration of the second bob
    float len;              // Length of the first string
    float len2;             // Length of the second string
    float mass;             // Mass of the first bob
    float mass2;            // Mass of the second bob
    float gravity;          // Gravitational acceleration
    float bobRadius;        // Radius of circular motion for the first bob
    float bob2Radius;       // Radius of circular motion for the second bob
    float bobSpeed;         // Angular speed for the first bob
    float bob2Speed;        // Angular speed for the second bob
    float time;             // Elapsed time
    float bob2Angle;        // Angle for the second bob's circular motion

public:
    Pendulum(float windowWidth, float windowHeight, float len, float len2, float mass, float mass2, float gravity) {
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
        piv = sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f);
        bob = sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f - 50.f); // Position the first bob slightly above the center
        bob2 = sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f - 100.f); // Position the second bob above the first bob
        circle = sf::CircleShape(10);
        circle2 = sf::CircleShape(10);
        angle = PI / 4;
        angle2 = PI / 4;
        angleVel = 0;
        angleVel2 = 0;
        angleAcc = 0;
        angleAcc2 = 0;
        this->len = len;
        this->len2 = len2;
        this->mass = mass;
        this->mass2 = mass2;
        this->gravity = gravity;
        bobRadius = 100.0f; // Adjust the radius of circular motion for the first bob
        bob2Radius = 150.0f; // Adjust the radius of circular motion for the second bob
        bobSpeed = 0.01f; // Adjust the angular speed for the first bob
        bob2Speed = 0.015f; // Adjust the angular speed for the second bob
        time = 0.0f;

        circle.setFillColor(sf::Color::Green);
        circle2.setFillColor(sf::Color::Green);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle2.setOrigin(circle2.getRadius(), circle2.getRadius());
        circle.setPosition(bob);
        circle2.setPosition(bob2);
        pivot.setOrigin(pivot.getSize().x / 2.f, pivot.getSize().y / 2.f);
        pivot.setPosition(piv);

        //Creating a visual representation of the pendulum by setting the endpoints of a line object.
        line1[0] = sf::Vertex(piv, sf::Color::Yellow);
        line1[1] = sf::Vertex(bob, sf::Color::Yellow);

        line2[0] = sf::Vertex(bob, sf::Color::White);
        line2[1] = sf::Vertex(bob2, sf::Color::White);
    }

    void update() {
        // this is the main formula which had come from the final expression of motion of double pendulum 
        // num1,num2,num3,num4 are numerator and den is denominator of the formula cause formula was so long we are just partationing the formula 
        float num1 = -gravity * (2 * mass + mass) * sin(angle);
        float num2 = -mass * gravity * sin(angle - 2 * angle2);
        float num3 = -2 * sin(angle - angle2) * mass;
        float num4 = angleVel2 * angleVel2 * len2 + angleVel * angleVel * len * cos(angle - angle2);
        float den = len * (2 * mass + mass - mass * cos(2 * angle - 2 * angle2));

        angleAcc = (num1 + num2 + num3 * num4) / den;
        angleVel += angleAcc;
        angle += angleVel;

        num1 = 2 * sin(angle - angle2);
        num2 = angleVel * angleVel * len * (mass + mass2);
        num3 = gravity * (mass + mass2) * cos(angle);
        num4 = angleVel2 * angleVel2 * len2 * mass2 * cos(angle - angle2);
        den = len2 * (2 * mass + mass2 - mass2 * cos(2 * angle - 2 * angle2));

        angleAcc2 = (num1 * (num2 + num3 + num4)) / den;
        angleVel2 += angleAcc2;
        angle2 += angleVel2;
        //angleVel *= 0.99f; // damping 

        bob.x = len * sin(angle) + piv.x;
        bob.y = len * cos(angle) + piv.y;

        bob2.x = len2 * sin(angle2) + bob.x;
        bob2.y = len2 * cos(angle2) + bob.y;

        circle.setPosition(bob);
        circle2.setPosition(bob2);
        line1[0] = sf::Vertex(piv, sf::Color::White);
        line1[1] = sf::Vertex(bob, sf::Color::White);

        line2[0] = sf::Vertex(bob, sf::Color::White);
        line2[1] = sf::Vertex(bob2, sf::Color::White);

        path.push_back(bob2);
        pathLine.append(sf::Vertex(bob2, sf::Color::White));
    }

    void render(sf::RenderWindow& window) {
        float centerX = windowWidth / 2.0f;
        float centerY = windowHeight / 2.0f;

        pivot.setPosition(centerX, centerY);
        circle.setPosition(bob.x, bob.y);
        circle2.setPosition(bob2.x, bob2.y);

        window.draw(pathLine);
        window.draw(line1, 2, sf::Lines);
        window.draw(circle);
        window.draw(circle2);
        window.draw(pivot);
        window.draw(line2, 2, sf::Lines);
    }
};

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    const unsigned int windowWidth = desktop.width;
    const unsigned int windowHeight = desktop.height;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Double Pendulum Simulation", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    Pendulum pend(windowWidth, windowHeight, 150, 155, 3, 3, 1);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        window.clear();
        pend.update();
        pend.render(window);
        window.display();
    }

    return 0;
}
