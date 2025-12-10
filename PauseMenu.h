// Galaxy Wars by Maaz & Danyal

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>


#define MAX_PAUSE_ITEMS 4

class PauseMenu 
{

private:

    int selectedItemIndex;
    sf::Font font;
    sf::Text menuItems[MAX_PAUSE_ITEMS];
    sf::Text titleText;
    sf::Clock animationClock; // For smooth animations

public:


    PauseMenu(float width, float height)
    {

        if (!font.loadFromFile("assets/Fonts/Arial.ttf")) {
            std::cerr << "Error loading font for PauseMenu!" << std::endl;
        }

        
        titleText.setFont(font);
        titleText.setString("- GAME PAUSED -");
        titleText.setFillColor(sf::Color::Yellow);
        titleText.setCharacterSize(50);
        titleText.setStyle(sf::Text::Bold);
        titleText.setOutlineColor(sf::Color::Black);
        titleText.setOutlineThickness(4);

        sf::FloatRect titleRect = titleText.getLocalBounds();
        titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
        titleText.setPosition(sf::Vector2f(width / 2.0f, height / 4.0f));

        std::string options[MAX_PAUSE_ITEMS] = { "RESUME", "SAVE GAME", "RESTART LEVEL", "ABORT" };

        for (int i = 0; i < MAX_PAUSE_ITEMS; i++) {
            menuItems[i].setFont(font);
            menuItems[i].setString(options[i]);
            menuItems[i].setCharacterSize(35);
            menuItems[i].setStyle(sf::Text::Bold);

            sf::FloatRect itemRect = menuItems[i].getLocalBounds();
            menuItems[i].setOrigin(itemRect.left + itemRect.width / 2.0f, itemRect.top + itemRect.height / 2.0f);
            menuItems[i].setPosition(sf::Vector2f(width / 2.0f, height / 2.5f + (i * 70)));
        }

        selectedItemIndex = 0;
    }

    ~PauseMenu() {}

    void draw(sf::RenderWindow& window) 
    {


        
        sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 200)); 
        window.draw(overlay);

        
        float time = animationClock.getElapsedTime().asSeconds();
        float yOffset = std::sin(time * 2) * 5.0f; 

        sf::Vector2f basePos(window.getSize().x / 2.0f, window.getSize().y / 4.0f);
        titleText.setPosition(basePos.x, basePos.y + yOffset);
        window.draw(titleText);

        
        for (int i = 0; i < MAX_PAUSE_ITEMS; i++) {
            if (i == selectedItemIndex) {
                
                menuItems[i].setFillColor(sf::Color::Cyan);
                menuItems[i].setScale(1.1f, 1.1f);
                menuItems[i].setOutlineColor(sf::Color::Blue);
                menuItems[i].setOutlineThickness(1);
            }
            else {

                menuItems[i].setFillColor(sf::Color(150, 150, 150));
                menuItems[i].setScale(1.0f, 1.0f);
                menuItems[i].setOutlineThickness(0);
            }
            window.draw(menuItems[i]);
        }
    }

    void moveUp() 
    {

        if (selectedItemIndex - 1 >= 0) selectedItemIndex--;
    }

    void moveDown() 
    {

        if (selectedItemIndex + 1 < MAX_PAUSE_ITEMS) selectedItemIndex++;
    }

    int getPressedItem() { return selectedItemIndex; }
};