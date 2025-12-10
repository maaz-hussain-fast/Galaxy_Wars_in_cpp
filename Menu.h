// Galaxy Wars by Maaz & Danyal

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define MAX_ITEMS 5

class Menu 
{


private:


    int selectedItemIndex;
    sf::Font font;
    sf::Text menuTexts[MAX_ITEMS];
    sf::RectangleShape menuBoxes[MAX_ITEMS];

    sf::Text titleText;
    sf::Text titleShadow;

    sf::Text productionText;
    sf::Clock animationClock;

public:


    Menu(float width, float height) 
    {

        if (!font.loadFromFile("assets/Fonts/Arial.ttf")) 
        {

            std::cerr << "Error loading font!" << std::endl;
        }


        titleText.setFont(font);
        titleText.setString("GALAXY WARS");
        titleText.setCharacterSize(100); 
        titleText.setFillColor(sf::Color::Red);
        titleText.setStyle(sf::Text::Bold | sf::Text::Italic);
        titleText.setOutlineThickness(4);
        titleText.setOutlineColor(sf::Color:: Black); 

        sf::FloatRect titleRect = titleText.getLocalBounds();
        titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
        titleText.setPosition(sf::Vector2f(width / 2.0f, height / 6.0f));

        titleShadow = titleText;
        titleShadow.setFillColor(sf::Color::Black);
        titleShadow.setOutlineThickness(0);
        titleShadow.setPosition(titleText.getPosition().x + 6, titleText.getPosition().y + 6);

        productionText.setFont(font);
        productionText.setString("- A MAAZ & DANYAL PRODUCTION -");
        productionText.setCharacterSize(30);
        productionText.setFillColor(sf::Color(255, 215, 0)); 
        productionText.setStyle(sf::Text::Bold);

        sf::FloatRect prodRect = productionText.getLocalBounds();
        productionText.setOrigin(prodRect.left + prodRect.width / 2.0f, prodRect.top + prodRect.height / 2.0f);
        productionText.setPosition(sf::Vector2f(width / 2.0f, height / 3.0f));


        std::string options[MAX_ITEMS] = { "START GAME", "LOAD GAME", "LEVELS", "HIGH SCORES", "EXIT" };
        float startY = height / 2.1f;

        for (int i = 0; i < MAX_ITEMS; i++) {
            
            menuBoxes[i].setSize(sf::Vector2f(300.f, 50.f));
            menuBoxes[i].setOrigin(150.f, 25.f); 
            menuBoxes[i].setPosition(sf::Vector2f(width / 2.0f, startY + (i * 65)));
            
            menuBoxes[i].setOutlineThickness(2);

            
            menuTexts[i].setFont(font);
            menuTexts[i].setString(options[i]);
            menuTexts[i].setCharacterSize(24);
            menuTexts[i].setStyle(sf::Text::Bold);

            sf::FloatRect textRect = menuTexts[i].getLocalBounds();
            menuTexts[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            menuTexts[i].setPosition(menuBoxes[i].getPosition());
        }

        selectedItemIndex = 0;
    }

    void draw(sf::RenderWindow& window) {
        float time = animationClock.getElapsedTime().asSeconds();

        
        float titleScale = 1.0f + 0.02f * std::sin(time * 3);
        titleText.setScale(titleScale, titleScale);
        titleShadow.setScale(titleScale, titleScale);

        window.draw(titleShadow);
        window.draw(titleText);
        window.draw(productionText);

        for (int i = 0; i < MAX_ITEMS; i++) {
            if (i == selectedItemIndex) {
                
                menuBoxes[i].setFillColor(sf::Color(0, 255, 255));
                menuBoxes[i].setOutlineColor(sf::Color::White);

                menuTexts[i].setFillColor(sf::Color::Black);

                float s = 1.05f + 0.03f * std::sin(time * 10);
                menuBoxes[i].setScale(s, s);
                menuTexts[i].setScale(s, s);
            }
            else {
                
                menuBoxes[i].setFillColor(sf::Color(0, 0, 0, 220));
                menuBoxes[i].setOutlineColor(sf::Color(0, 255, 255)); 

                menuTexts[i].setFillColor(sf::Color::White);

                menuBoxes[i].setScale(1.f, 1.f);
                menuTexts[i].setScale(1.f, 1.f);
            }

            window.draw(menuBoxes[i]);
            window.draw(menuTexts[i]);
        }
    }




    void moveUp() { if (selectedItemIndex - 1 >= 0) selectedItemIndex--; }
    void moveDown() { if (selectedItemIndex + 1 < MAX_ITEMS) selectedItemIndex++; }
    int getPressedItem() { return selectedItemIndex; }
};