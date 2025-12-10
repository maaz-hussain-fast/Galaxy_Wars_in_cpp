// Galaxy Wars by Maaz & Danyal


#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define MAX_LEVELS 3

class LevelMenu 
{


private:


    int selectedItemIndex;
    sf::Font font;
    sf::Text menuTexts[MAX_LEVELS];
    sf::RectangleShape menuBoxes[MAX_LEVELS];
    sf::Text titleText;
    sf::Clock animClock;


public:

    LevelMenu(float width, float height) {
        if (!font.loadFromFile("assets/Fonts/Arial.ttf")) std::cerr << "Error loading font!" << std::endl;

        
        titleText.setFont(font);
        titleText.setString("DEPLOYMENT ZONE"); 
        titleText.setFillColor(sf::Color::Cyan);
        titleText.setCharacterSize(50);
        titleText.setStyle(sf::Text::Bold);
        titleText.setOutlineColor(sf::Color::Black);
        titleText.setOutlineThickness(3);

        sf::FloatRect titleRect = titleText.getLocalBounds();
        titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
        titleText.setPosition(sf::Vector2f(width / 2.0f, height / 6.0f));

        std::string options[MAX_LEVELS] = { "LEVEL 1: ROOKIE", "LEVEL 2: VETERAN", "LEVEL 3: BOSS NIGHTMARE" };

        for (int i = 0; i < MAX_LEVELS; i++) 
        {


            menuBoxes[i].setSize(sf::Vector2f(450.f, 60.f));
            menuBoxes[i].setOutlineThickness(2);
            menuBoxes[i].setOrigin(225.f, 30.f);
            menuBoxes[i].setPosition(sf::Vector2f(width / 2.0f, height / 2.5f + (i * 80)));

            menuTexts[i].setFont(font);
            menuTexts[i].setString(options[i]);
            menuTexts[i].setCharacterSize(30);
            menuTexts[i].setStyle(sf::Text::Bold);

            sf::FloatRect textRect = menuTexts[i].getLocalBounds();
            menuTexts[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            menuTexts[i].setPosition(menuBoxes[i].getPosition());
        }


        selectedItemIndex = 0;
    }

    void draw(sf::RenderWindow& window)
    {


        window.draw(titleText);
        float time = animClock.getElapsedTime().asSeconds();

        for (int i = 0; i < MAX_LEVELS; i++) 
        
        {
            if (i == selectedItemIndex) {
                
                float s = 1.0f + 0.02f * sin(time * 8);
                menuBoxes[i].setScale(s, s);
                menuBoxes[i].setFillColor(sf::Color(0, 100, 255, 150)); 
                menuBoxes[i].setOutlineColor(sf::Color::Cyan);
                menuTexts[i].setFillColor(sf::Color::White);
            }

            else 
            
            {
                menuBoxes[i].setScale(1.f, 1.f);
                menuBoxes[i].setFillColor(sf::Color(20, 20, 20, 200));
                menuBoxes[i].setOutlineColor(sf::Color(100, 100, 100));
                menuTexts[i].setFillColor(sf::Color(180, 180, 180));
            }
            window.draw(menuBoxes[i]);
            window.draw(menuTexts[i]);
        }
    }


    void moveUp() { if (selectedItemIndex - 1 >= 0) selectedItemIndex--; }
    void moveDown() { if (selectedItemIndex + 1 < MAX_LEVELS) selectedItemIndex++; }
    int getPressedItem() { return selectedItemIndex; }
};