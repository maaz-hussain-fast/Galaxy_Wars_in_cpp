// Galaxy Wars by Maaz & Danyal


#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


class Asteroid 
{

private:


    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    bool active;


public:


    Asteroid() {

        if (!texture.loadFromFile("assets/Meteors/asteroid.png")) {
            
            if (!texture.loadFromFile("assets/asteroid.png")) {
                std::cerr << "Error loading asteroid.png!" << std::endl;
            }
        }

        sprite.setTexture(texture);
        sprite.setScale(0.125f, 0.125f);

        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        speed = 0.f;
        active = false;
    }

    ~Asteroid() {}

    void spawn(float startX, float startY, float moveSpeed) {



        sprite.setPosition(startX, startY);
        speed = moveSpeed;
        active = true;
        sprite.setRotation(static_cast<float>(rand() % 360));
    }

    void update() {


        if (active) {

            sprite.move(0.f, speed);

            sprite.rotate(1.0f);

            
            float bottom = sprite.getPosition().y + (sprite.getGlobalBounds().height / 2);

            if (bottom >= 480.f) {
                active = false;
            }

            if (sprite.getPosition().y > 600) {
                active = false;
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        if (active) {
            window.draw(sprite);
        }
    }

    sf::FloatRect getBounds() {
        return sprite.getGlobalBounds();
    }

    bool isActive() { return active; }
    void deactivate() { active = false; }
};