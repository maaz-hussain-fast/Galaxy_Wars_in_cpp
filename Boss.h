// Galaxy Wars by Maaz & Danyal


#pragma once
#include "Enemy.h" 

class Boss : public Enemy {

private:
    

    int health;
    bool defeated;

    sf::Vector2f targetPosition;
    sf::Clock stateTimer;
    bool isIdle;
    float waitTime;

    sf::Clock fireClock;
    float fireRate;

public:
    Boss() : Enemy()
    { 
        
        if (!texture.loadFromFile("assets/boss.png")) {

            if (!texture.loadFromFile("assets/sprites/boss.png")) {

                std::cerr << "Error loading boss.png!" << std::endl;
            }
        }

        sprite.setTexture(texture);

        
        sprite.setScale(0.7f, 0.7f);
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        active = false;
        defeated = false;
        health = 10;
        speed = 3.0f; 

        isIdle = false;
        waitTime = 1.0f;

        fireRate = 0.8f;
    }

    ~Boss() {}

    void spawn() 
    {
        if (!defeated) 
        {



            sprite.setPosition(400.f, -100.f);
            active = true;
            health = 10;
            pickNewTarget();
            fireClock.restart();

        }
    }

    void pickNewTarget()
    {


        float x = static_cast<float>(rand() % 700 + 50);
        float y = static_cast<float>(rand() % 400 + 50);
        targetPosition = sf::Vector2f(x, y);
        isIdle = false;
    }

    bool shouldFire() 
    {

        if (fireClock.getElapsedTime().asSeconds() > fireRate) 
        {

            fireClock.restart();
            return true;
        }
        return false;
    }


    void update() override 
    {

        if (active) {

            if (isIdle) {
                if (stateTimer.getElapsedTime().asSeconds() > waitTime) {
                    pickNewTarget();
                }
            }
            else {
                sf::Vector2f direction = targetPosition - sprite.getPosition();

                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                if (distance < 5.0f) {

                    isIdle = true;
                    waitTime = static_cast<float>(rand() % 2 + 1);
                    stateTimer.restart();
                }
                else {

                    direction /= distance;
                    sprite.move(direction * speed);
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) const override 
    {
        if (active) window.draw(sprite);
    }

    void takeDamage() 
    {
        health--;
        sprite.setColor(sf::Color::Red);
    }

    void updateColor() 
    {
        if (sprite.getColor() != sf::Color::White) 
        {


            sf::Color c = sprite.getColor();
            if (c.g < 255) c.g += 5;
            if (c.b < 255) c.b += 5;
            sprite.setColor(c);
        }
    }

    bool isDefeated() { return defeated; }

    int getHealth() { return health; }

    void kill() 
    {


        active = false;
        defeated = true;
    }

    void reset() 
    {


        active = false;
        defeated = false;
        health = 10;
    }
};