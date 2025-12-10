// Galaxy Wars by Maaz & Danyal

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Missile 
{


private:


    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    bool active;

public:


    Missile() 
    {
        
        if (!texture.loadFromFile("assets/missiles/missile2.png")) {
            
            if (!texture.loadFromFile("assets/missile2.png")) 
            {
                std::cerr << "Error loading missile2.png! Check assets/missiles/" << std::endl;
            }
        }

        sprite.setTexture(texture);
        sprite.setScale(0.5f, 0.5f);

        active = false;
        speed = -8.0f; 
    }

    ~Missile() {}

    void fire(float startX, float startY)
    
    {
        sprite.setPosition(startX - (sprite.getGlobalBounds().width / 2), startY);
        active = true;
    }

    void update() 
    {
        if (active) {
            sprite.move(0.f, speed);

            
            if (sprite.getPosition().y < -50)
            {
                active = false;
            }
        }
    }

    void draw(sf::RenderWindow& window) 
    {
        if (active) {
            window.draw(sprite);
        }
    }

    sf::FloatRect getBounds()
    {
        return sprite.getGlobalBounds();
    }

    bool isActive()
    {
        return active;
    }

    void deactivate() 
    {

        active = false;
    }
};