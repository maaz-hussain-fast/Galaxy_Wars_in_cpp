// Galaxy Wars by Maaz & Danyal

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


class PowerUp 
{

private:

    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    bool active;
    int type; 

public:


    PowerUp() 
    {
        active = false;
        speed = 2.0f;
        type = 0;
    }

    ~PowerUp() {}

    void spawn(float startX, float startY, int powerUpType)
    {


        active = true;
        type = powerUpType;
        sprite.setPosition(startX, startY);


        std::string filename;
        if (type == 0) filename = "assets/sprites/shield.png";
        else if (type == 1) filename = "assets/sprites/pill.png";
        else if (type == 2) filename = "assets/sprites/star.png";   
        else if (type == 3) filename = "assets/sprites/freeze.png"; 

        
        if (!texture.loadFromFile(filename))
        {

        }

        sprite.setTexture(texture);

        
        sprite.setScale(0.5f, 0.5f);

        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    }

    void update() 
    {

        if (active) {
            sprite.move(0.f, speed); 
            sprite.rotate(2.0f);

            
            if (sprite.getPosition().y > 480.f) 
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




    sf::FloatRect getBounds() { return sprite.getGlobalBounds(); }
    bool isActive() { return active; }
    void deactivate() { active = false; }
    int getType() { return type; }
};