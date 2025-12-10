// Galaxy Wars by Maaz & Danyal

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


#define MAX_SLOTS 3

class SlotMenu
{


private:
    
    int selectedItemIndex;
    sf::Font font;
    sf::Text menuTexts[MAX_SLOTS];
    sf::RectangleShape menuBoxes[MAX_SLOTS];
    sf::Text titleText;
    bool isSavingMode;
    sf::Clock animClock;

public:

    SlotMenu(float width, float height) 
    {

        if (!font.loadFromFile("assets/Fonts/Arial.ttf")) {
            std::cerr << "Error loading font in SlotMenu!" << std::endl;
        }

        titleText.setFont(font);
        titleText.setString("MEMORY BANKS");
        titleText.setFillColor(sf::Color::Cyan);
        titleText.setCharacterSize(45);
        titleText.setStyle(sf::Text::Bold);

        sf::FloatRect titleRect = titleText.getLocalBounds();
        titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
        titleText.setPosition(sf::Vector2f(width / 2.0f, height / 6.0f));


        for (int i = 0; i < MAX_SLOTS; i++) {
            menuBoxes[i].setSize(sf::Vector2f(500.f, 70.f)); // Taller, nicer boxes
            menuBoxes[i].setOrigin(250.f, 35.f);
            menuBoxes[i].setPosition(sf::Vector2f(width / 2.0f, height / 2.5f + (i * 90)));

            menuTexts[i].setFont(font);
            menuTexts[i].setCharacterSize(26);
        }

        selectedItemIndex = 0;
        isSavingMode = false;
        refreshSlots();
    }

    void setMode(bool saving) 
    {

        isSavingMode = saving;
        if (isSavingMode) {
            titleText.setString("OVERWRITE DATA?");
            titleText.setFillColor(sf::Color(255, 100, 100)); 
        }
        else {


            titleText.setString("SELECT PROGRESS");
            titleText.setFillColor(sf::Color::Cyan); 
        }

        sf::FloatRect titleRect = titleText.getLocalBounds();
        titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);

        refreshSlots();
    }

    void refreshSlots() 
    {

        for (int i = 0; i < MAX_SLOTS; i++) 
        {

            std::string filename = "save_" + std::to_string(i + 1) + ".dat";
            std::ifstream file(filename);

            std::string label = "SLOT " + std::to_string(i + 1);

            
            if (file.good()) {

                label += "      [ SAVED ]";

            }
            else {
                label += "      [ . . . ]";
            }
            file.close();

            menuTexts[i].setString(label);

            
            sf::FloatRect textRect = menuTexts[i].getLocalBounds();
            menuTexts[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            menuTexts[i].setPosition(menuBoxes[i].getPosition());
        }
    }

    void draw(sf::RenderWindow& window)
    {


        window.draw(titleText);
        float time = animClock.getElapsedTime().asSeconds();

        for (int i = 0; i < MAX_SLOTS; i++) {

            bool hasData = menuTexts[i].getString().find("READY") != std::string::npos;

            if (i == selectedItemIndex) 
            
            {
                
                float s = 1.0f + 0.02f * std::sin(time * 5);
                menuBoxes[i].setScale(s, s);

                menuBoxes[i].setFillColor(sf::Color(0, 0, 0, 150)); 
                menuBoxes[i].setOutlineThickness(2);

                if (isSavingMode) {
                    menuBoxes[i].setOutlineColor(sf::Color::Red);
                    menuTexts[i].setFillColor(sf::Color::White);
                }
                else {
                    menuBoxes[i].setOutlineColor(sf::Color::Cyan);
                    menuTexts[i].setFillColor(sf::Color::White);
                }
            }
            else {
                
                menuBoxes[i].setScale(1.f, 1.f);
                menuBoxes[i].setFillColor(sf::Color(0, 0, 0, 100));
                menuBoxes[i].setOutlineThickness(1);
                menuBoxes[i].setOutlineColor(sf::Color(50, 50, 50));

                if (hasData) menuTexts[i].setFillColor(sf::Color(0, 200, 0));
                else menuTexts[i].setFillColor(sf::Color(80, 80, 80));
            }

            window.draw(menuBoxes[i]);
            window.draw(menuTexts[i]);
        }
    }



    void moveUp() { if (selectedItemIndex - 1 >= 0) selectedItemIndex--; }
    void moveDown() { if (selectedItemIndex + 1 < MAX_SLOTS) selectedItemIndex++; }
    int getPressedItem() { return selectedItemIndex; }
};