// Galaxy Wars by Maaz & Danyal

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Dashboard
{


private:

    sf::Font font;

    sf::Text scoreText;
    sf::Text livesText;
    sf::Text timerText;
    sf::Text powerupsText;
    sf::Text speedText;
    sf::Text levelTitleText;

    int score;
    int lives;
    int powerups;
    float width;

    
    sf::Clock gameClock;        
    sf::Clock pauseClock;       
    sf::Time totalPausedTime;   
    bool isPaused;

public:

    Dashboard(float w, float h) 
    {

        width = w;
        if (!font.loadFromFile("assets/Fonts/Arial.ttf")) 
        {

            std::cerr << "Error loading font for Dashboard!" << std::endl;
        }

        score = 0;
        lives = 3;
        powerups = 0;

        
        totalPausedTime = sf::Time::Zero;
        isPaused = false;

        float dashboardTop = 480.f;

        levelTitleText.setFont(font);
        levelTitleText.setString("LEVEL : 1");
        levelTitleText.setCharacterSize(35);
        levelTitleText.setFillColor(sf::Color::Yellow);
        levelTitleText.setStyle(sf::Text::Bold);
        centerText(levelTitleText, dashboardTop + 30.f);

        speedText.setFont(font);
        speedText.setString("Speed : 1x");
        speedText.setCharacterSize(20);
        speedText.setFillColor(sf::Color::White);
        centerText(speedText, dashboardTop + 70.f);

        speedText.setFont(font);
        speedText.setString("Speed : Normal"); 
        speedText.setCharacterSize(20);
        speedText.setFillColor(sf::Color::White);
        centerText(speedText, dashboardTop + 70.f);

        int fontSize = 24;
        sf::Color textColor = sf::Color::White;

        scoreText.setFont(font);
        scoreText.setCharacterSize(fontSize);
        scoreText.setFillColor(textColor);
        scoreText.setPosition(30.f, dashboardTop + 20.f);

        livesText.setFont(font);
        livesText.setCharacterSize(fontSize);
        livesText.setFillColor(textColor);
        livesText.setPosition(30.f, dashboardTop + 60.f);

        timerText.setFont(font);
        timerText.setCharacterSize(fontSize);
        timerText.setFillColor(textColor);
        timerText.setPosition(650.f, dashboardTop + 20.f);

        powerupsText.setFont(font);
        powerupsText.setCharacterSize(fontSize);
        powerupsText.setFillColor(textColor);
        powerupsText.setPosition(600.f, dashboardTop + 60.f);

        updateText();
    }

    ~Dashboard() {}

    void centerText(sf::Text& text, float y) 
    
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        text.setPosition(width / 2.0f, y);
    }

    
    void setScore(int s) 
    
    {
        score = s;
        updateText();
    }

    void setLives(int l)
    
    {
        lives = l;
        updateText();
    }
    
    void startPause() 
    
    {
        if (!isPaused) 
        {
            isPaused = true;
            pauseClock.restart(); 
        }
    }

    void addLife()
    {
        lives++;
        
        if (lives > 5) lives = 5;
        updateText();
    }

    void addPowerupCount() 
    
    {
        powerups++;
        updateText();
    }

    void endPause() 
    
    {
        if (isPaused)
        {
            isPaused = false;
            
            totalPausedTime += pauseClock.getElapsedTime();
        }
    }

    std::string formatTime() {


        sf::Time elapsed = gameClock.getElapsedTime() - totalPausedTime;

        if (elapsed < sf::Time::Zero) elapsed = sf::Time::Zero;

        int totalSeconds = static_cast<int>(elapsed.asSeconds());
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        std::string m_str = std::to_string(minutes);
        std::string s_str = std::to_string(seconds);

        if (minutes < 10) m_str = "0" + m_str;
        if (seconds < 10) s_str = "0" + s_str;

        return m_str + ":" + s_str;
    }

    void update() 
    
    {
        if (!isPaused)
        {
            updateText();
        }
    }

    void updateText()
    {

        scoreText.setString(formatScore(score));
        livesText.setString("Lives : " + std::to_string(lives));
        timerText.setString(formatTime());
        powerupsText.setString("Powerups : " + std::to_string(powerups));
    }

    void updateLevel(int level) 
    {
        levelTitleText.setString("LEVEL : " + std::to_string(level));

        
        centerText(levelTitleText, 480.f + 30.f);
    }

    std::string formatScore(int s)
    {

        std::string s_str = std::to_string(s);
        while (s_str.length() < 6) s_str = "0" + s_str;
        return "Score : " + s_str;
    }

    void draw(sf::RenderWindow& window)
    
    {
        window.draw(levelTitleText);
        window.draw(speedText);
        window.draw(scoreText);
        window.draw(livesText);
        window.draw(timerText);
        window.draw(powerupsText);
    }

    void reset() 
    
    {
        score = 0;
        lives = 3;
        powerups = 0;
        gameClock.restart();
        totalPausedTime = sf::Time::Zero; 
        isPaused = false;
        updateLevel(1);
    }



    void addScore(int points) { score += points; }
    int getScore() { return score; }
    int getLives() { return lives; }
    void loseLife() { if (lives > 0) lives--; }
};