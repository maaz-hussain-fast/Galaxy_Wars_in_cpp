// Galaxy Wars by Maaz & Danyal

#pragma once
#include <SFML/Graphics.hpp>

namespace GalaxyWars 

{

    class GameObject {


    protected:


        sf::Sprite sprite;
        bool active;

    public:


        GameObject() { active = false; }

        virtual ~GameObject() {}

        virtual void update() = 0;

        virtual void draw(sf::RenderWindow& window) const 
        {

            if (active) window.draw(sprite);
        }

        bool isActive() const { return active; }
        void deactivate() { active = false; }

        sf::FloatRect getBounds() const {


            return sprite.getGlobalBounds();
        }

        sf::Sprite& getSprite() { return sprite; }
    };
}