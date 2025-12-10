// Galaxy Wars by Maaz & Danyal

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>


class GameOverScreen {

private:


    sf::Font font;
    sf::Text titleText;
    sf::Text scoreText;
    sf::Text infoText;
    float width, height;
    bool isVictory;
    sf::Clock animClock;

public:


    GameOverScreen(float w, float h)
    {


        width = w;
        height = h;
        isVictory = false;


        if (!font.loadFromFile("assets/Fonts/Arial.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
        }

        titleText.setFont(font);
        titleText.setCharacterSize(70);
        titleText.setStyle(sf::Text::Bold);
        titleText.setOutlineThickness(4);

        scoreText.setFont(font);
        scoreText.setCharacterSize(40);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setOutlineColor(sf::Color::Black);
        scoreText.setOutlineThickness(2);

        infoText.setFont(font);
        infoText.setString("Returning to Base...");
        infoText.setCharacterSize(20);
        infoText.setFillColor(sf::Color(200, 200, 200));
        centerText(infoText, height * 0.85f);
    }

    void setVictory(bool won) 
    
    {


        isVictory = won;

        if (won) 
        
        {
            titleText.setString("VICTORY");
            titleText.setFillColor(sf::Color::Green);
            titleText.setOutlineColor(sf::Color::White);
        }
        else 
        
        {
            titleText.setString("WASTED");
            titleText.setFillColor(sf::Color::Red);
            titleText.setOutlineColor(sf::Color::Black);
        }


        centerText(titleText, height / 3.0f);
    }

    void setFinalScore(int score) 
    {

        scoreText.setString("Final Score: " + std::to_string(score));
        centerText(scoreText, height / 2.0f);
    }

    void centerText(sf::Text& text, float y) 
    {

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        text.setPosition(width / 2.0f, y);
    }

    void draw(sf::RenderWindow& window) 
    {

       
        sf::RectangleShape overlay(sf::Vector2f(width, height));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        float time = animClock.getElapsedTime().asSeconds();

        
        if (isVictory) 
        {

            
            float r = 128 + 127 * sin(time * 3);
            float g = 255;
            float b = 128 + 127 * cos(time * 3);

            titleText.setFillColor(sf::Color((sf::Uint8)r, (sf::Uint8)g, (sf::Uint8)b));

            float scale = 1.0f + 0.05f * sin(time * 5);
            titleText.setScale(scale, scale);
        }

        else {
            
            float scale = 1.0f + 0.02f * sin(time * 2);
            titleText.setScale(scale, scale);
        }

        window.draw(titleText);
        window.draw(scoreText);
        window.draw(infoText);

    }
};