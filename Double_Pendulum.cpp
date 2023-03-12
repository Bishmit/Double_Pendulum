#include<SFML/Graphics.hpp>
#include<cmath>
#define PI 3.14159
#define width 800.f
#define height 800.f

class Pendulum {
    sf::Vector2f piv;
    sf::Vector2f bob;
    sf::Vector2f bob2;
    sf::RectangleShape pivot;
    sf::CircleShape circle;
    sf::CircleShape circle2;
    sf::Vertex line1[2];
    sf::Vertex line2[2];
    float angle; float angle2;
    float angleVel; float angleVel2;
    float angleAcc; float angleAcc2;
    float len; float len2;
    float mass; float mass2;
    float gravity;
public:
    Pendulum(float origin_x, float origin_y, float len, float len2, float mass, float mass2, float gravity) {
        piv = sf::Vector2f(origin_x, origin_y);
        bob = sf::Vector2f(width / 2.f, 50.f);
        bob2 = sf::Vector2f(width / 2.f, 100.f);
        circle = sf::CircleShape(10);// gives radius to the bob 
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

        circle.setFillColor(sf::Color::Green);
        circle2.setFillColor(sf::Color::Green);
        pivot.setSize(sf::Vector2f(10.f, 10.f));
        pivot.setFillColor(sf::Color::Yellow);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle2.setOrigin(circle2.getRadius(), circle2.getRadius());
        circle.setPosition(bob);
        circle.setPosition(bob2);
        pivot.setOrigin(pivot.getSize().x / 2.f, pivot.getSize().y / 2.f); // set pivot origin at the center
        pivot.setPosition(piv);

        //Creating a visual representation of the pendulum by setting the endpoints of a line object.
        line1[0] = sf::Vertex(piv, sf::Color::Yellow);
        line1[1] = sf::Vertex(bob, sf::Color::Yellow);

        line2[0] = sf::Vertex(bob, sf::Color::White);
        line2[1] = sf::Vertex(bob2, sf::Color::White);
    }

    void update() {
        // this is the main formula which had come from the final expression of motion of double pendulum 
        // num1,num2,num3,num4 are numerator and den is denominator of the formula cause formula was so long we are just dividing formula 
        float num1 = -gravity * (2 * mass + mass) * sin(angle);
        float num2 = -mass * gravity * sin(angle - 2 * angle2);
        float num3 = -2 * sin(angle - angle2) * mass;
        float num4 = angleVel2 * angleVel2 * len2 + angleVel * angleVel * len * cos(angle - angle2);
        float den = len * (2 * mass + mass - mass * cos(2 * angle - 2 * angle2));

        angleAcc = (num1 + num2 + num3 * num4) / den;
        angleVel += angleAcc;
        angle += angleVel;
        //angleVel *= 0.99f;

        num1 = 2 * sin(angle - angle2);
        num2 = angleVel * angleVel * len * (mass + mass2);
        num3 = gravity * (mass + mass2) * cos(angle);
        num4 = angleVel2 * angleVel2 * len2 * mass2 * cos(angle - angle2);
        den = len2 * (2 * mass + mass2 - mass2 * cos(2 * angle - 2 * angle2));

        angleAcc2 = (num1 * (num2 + num3 + num4)) / den;
        angleVel2 += angleAcc2;
        angle2 += angleVel2;
        // angleVel2 *= 0.99f;

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

    }


    void render(sf::RenderWindow& window) {
        window.draw(line1, 2, sf::Lines);
        window.draw(circle);
        window.draw(circle2);
        window.draw(pivot);
        window.draw(line2, 2, sf::Lines);

    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Simple Pendulum");
    window.setFramerateLimit(60);

    Pendulum pend(width / 2.f, 250, 150, 155, 3, 3, 1);

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
