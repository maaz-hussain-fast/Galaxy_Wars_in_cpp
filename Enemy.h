// Galaxy Wars by Maaz & Danyal


#pragma once
#include "GameObject.h"
#include <cmath>

using namespace GalaxyWars;

class Enemy : public GameObject 

{
protected: 

    sf::Texture texture;
    float speed;
    int level;
    float initialX;

    static int totalEnemiesSpawned;

public:

    Enemy() {


        if (!texture.loadFromFile("assets/sprites/enemy.png")) {
            
            texture.loadFromFile("assets/enemy.png");
        }


        sprite.setTexture(texture);
        sprite.setScale(0.5f, 0.5f);
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        speed = 0.f;
        active = false;
    }

    virtual ~Enemy() {}

    

    virtual void spawn(float startX, float startY, float moveSpeed, int currentLevel) 
    
    {
        sprite.setPosition(startX, startY);
        speed = moveSpeed;
        level = currentLevel;
        initialX = startX;
        active = true;

        totalEnemiesSpawned++;
    }

    void update() override 
    
    {
        if (active) 
        
        {
            float newY = sprite.getPosition().y + speed;

            if (level == 3)
            {
                
                float newX = initialX + std::sin(newY * 0.05f) * 50.0f;
                sprite.setPosition(newX, newY);
            }
            else {
                sprite.setPosition(sprite.getPosition().x, newY);
            }

            if (sprite.getPosition().y >= 480.f) {
                active = false;
            }
        }
    }

    static int getTotalSpawned() 
    
    {

        return totalEnemiesSpawned;
    }
};

int Enemy::totalEnemiesSpawned = 0;