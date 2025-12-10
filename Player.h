// Galaxy Wars by Maaz & Danyal

#pragma once
#include "GameObject.h" 
#include "PowerUp.h"
#include <iostream>

using namespace GalaxyWars; 

class Player : public GameObject
{

private:


    sf::Texture texture;
    float movementSpeed;
    bool textureLoaded;

    bool isShielded;
    sf::Clock shieldTimer;

    
    friend class Game;

public:


    Player() 
    {


        textureLoaded = false;
        isShielded = false;
        active = true; 

        if (texture.loadFromFile("assets/ships/player.png")) { textureLoaded = true; }
        else if (texture.loadFromFile("assets/player.png")) { textureLoaded = true; }
        else { std::cerr << "CRITICAL ERROR: Player texture not found!" << std::endl; }

        if (textureLoaded) 
        {

            sprite.setTexture(texture);
            sprite.setScale(0.5f, 0.5f);
        }

        movementSpeed = 5.0f;
        sprite.setPosition(400.f, 350.f);
    }

    
    void operator+(PowerUp& powerup) 
    {


        if (!powerup.isActive()) return;

        int type = powerup.getType();
        if (type == 0) activateShield();
        
    }

    void update() override {

        
        if (isShielded) {
            if (shieldTimer.getElapsedTime().asSeconds() > 10.0f) deactivateShield();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) sprite.move(-movementSpeed, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) sprite.move(movementSpeed, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) sprite.move(0.f, -movementSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) sprite.move(0.f, movementSpeed);


        if (sprite.getPosition().x < 0) sprite.setPosition(0.f, sprite.getPosition().y);
        if (sprite.getPosition().x + 50 > 800) sprite.setPosition(750.f, sprite.getPosition().y);
        if (sprite.getPosition().y < 0) sprite.setPosition(sprite.getPosition().x, 0.f);
        if (sprite.getPosition().y + 50 > 480) sprite.setPosition(sprite.getPosition().x, 430.f);
    }


    void draw(sf::RenderWindow& window) const override {
        window.draw(sprite);
    }

    void activateShield() 
    {


        isShielded = true;
        shieldTimer.restart();
        sprite.setColor(sf::Color::Cyan);
    }

    void deactivateShield()
    {


        isShielded = false;
        sprite.setColor(sf::Color::White);
    }

    bool hasShield() const { return isShielded; } 

    void resetPosition()
    {


        sprite.setPosition(400.f, 350.f);
        deactivateShield();
    }
};