// Galaxy Wars by Maaz & Danyal


#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


class Bullet 
{


private:


    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    bool active;

public:


    Bullet() {
        
        if (!texture.loadFromFile("assets/missiles/bullet.png")) {
            
            if (!texture.loadFromFile("assets/bullet.png")) 
            {

                std::cerr << "Error loading bullet.png! Check assets/missiles/" << std::endl;
            }
        }

        sprite.setTexture(texture);

        
        sprite.setScale(0.5f, 0.5f);

        active = false;
        speed = 3.0f; 
    }

    ~Bullet() {}

    void spawn(float startX, float startY)
    {

        sprite.setPosition(startX, startY);
        active = true;
    }

    void update() 
    {

        if (active) 
        {

            sprite.move(0.f, speed); 

            
            float bottomEdge = sprite.getPosition().y + sprite.getGlobalBounds().height;

            if (bottomEdge >= 480.f) 
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