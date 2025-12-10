// Galaxy Wars by Maaz & Danyal

#pragma once

#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#include <iostream>
#include <cstdlib>

#include <ctime>
#include <algorithm>

#include <iomanip>
#include <sstream>



#include "GameException.h"
#include "GameObject.h"
#include "ObjectPool.h"
#include "Menu.h"
#include "PauseMenu.h"
#include "Player.h"
#include "Dashboard.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Missile.h"
#include "Asteroid.h"
#include "GameOverScreen.h"
#include "SoundManager.h"
#include "LevelMenu.h"
#include "PowerUp.h"
#include "Boss.h"
#include "SlotMenu.h"

using namespace GalaxyWars;

#define MAX_ENEMIES 10
#define MAX_BULLETS 20
#define MAX_MISSILES 50
#define MAX_ASTEROIDS 5
#define MAX_POWERUPS 3
#define MAX_HIGH_SCORES 50

struct HighScoreEntry
{
    std::string name;
    int score;
    std::string timePlayed;
};

class Game
{

private:
    
    
    sf::RenderWindow window;

    Menu* menu;
    LevelMenu* levelMenu;
    PauseMenu* pauseMenu;
    GameOverScreen* gameOverScreen;
    SlotMenu* slotMenu;
    Dashboard* dashboard;

    Player* player;
    SoundManager* soundManager;
    Boss* boss;

    sf::Texture menuBgTexture;
    sf::Sprite menuBgSprite;

    HighScoreEntry topScores[MAX_HIGH_SCORES];
    int scoresLoadedCount;
    float scrollOffset;
    sf::Text scoreDisplayTitle;
    sf::Text scoreListText;
    sf::Text scoreColumnHeader;
    sf::RectangleShape scoreBoardBackground;

    ObjectPool<PowerUp, MAX_POWERUPS> powerups;
    sf::Clock powerupSpawnClock;

    bool isGameFrozen;
    sf::Clock freezeTimer;

    Enemy* enemies[MAX_ENEMIES];
    sf::Clock enemySpawnClock;

    std::string playerName;
    sf::Text namePromptText;
    sf::Text nameInputText;
    sf::Text welcomeText;
    sf::Font font;

    sf::Clock welcomeStateClock;
    sf::Clock typewriterClock;
    std::string fullWelcomeMessage;
    std::string currentWelcomeDisplay;
    size_t charIndex;

    Asteroid* asteroids[MAX_ASTEROIDS];
    sf::Clock asteroidSpawnClock;

    Bullet* bullets[MAX_BULLETS];
    Missile* missiles[MAX_MISSILES];

    int gameState;
    int currentLevel;
    int levelKills;
    sf::Clock gameOverClock;

    sf::Clock totalGameTimeClock;

    void startGame(int startLevel)
    {

        gameState = 1;
        currentLevel = startLevel;
        levelKills = 0;

        dashboard->reset();
        dashboard->updateLevel(currentLevel);

        player->resetPosition();
        resetEntities();

        boss->reset();
        isGameFrozen = false;

        soundManager->stopMenuMusic();
        soundManager->startGameMusic();

        totalGameTimeClock.restart();
    }

    void saveScoreToFile()
    {


        try
        {
            int totalSeconds = static_cast<int>(totalGameTimeClock.getElapsedTime().asSeconds());
            int mins = totalSeconds / 60;
            int secs = totalSeconds % 60;
            std::ostringstream timeStream;
            timeStream << mins << "m " << secs << "s";
            std::string timeStr = timeStream.str();

            std::ofstream file("scores.txt", std::ios::app);

            if (!file.is_open())
            {
                throw GameException("Critical Error: Unable to open scores.txt for writing.");
            }

            file << "Name: " << playerName << " | Score: " << dashboard->getScore() << " | Time: " << timeStr << std::endl;
            file.close();
            std::cout << "Score saved successfully!" << std::endl;
        }
        catch (const GameException& e)
        {
            std::cerr << "[File Error]: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Standard Error]: " << e.what() << std::endl;
        }
    }

    void saveGame(int slotIndex)
    {

        std::string filename = "save_" + std::to_string(slotIndex + 1) + ".dat";
        std::ofstream file(filename, std::ios::binary);
        if (file.is_open())
        {
            size_t nameLen = playerName.size();
            file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
            file.write(playerName.c_str(), nameLen);

            int score = dashboard->getScore();
            int lives = dashboard->getLives();

            file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
            file.write(reinterpret_cast<const char*>(&score), sizeof(score));
            file.write(reinterpret_cast<const char*>(&lives), sizeof(lives));

            file.close();
            std::cout << "Saved to " << filename << std::endl;
        }
    }

    bool loadGame(int slotIndex)

    {
        std::string filename = "save_" + std::to_string(slotIndex + 1) + ".dat";
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open()) return false;

        size_t nameLen;
        if (!file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen))) return false;

        playerName.resize(nameLen);
        file.read(&playerName[0], nameLen);

        int level, score, lives;
        file.read(reinterpret_cast<char*>(&level), sizeof(level));
        file.read(reinterpret_cast<char*>(&score), sizeof(score));
        file.read(reinterpret_cast<char*>(&lives), sizeof(lives));

        file.close();

        currentLevel = level;
        startGame(currentLevel);
        dashboard->setScore(score);
        dashboard->setLives(lives);
        return true;
    }

    void loadHighScores()
    {
        try
        {
            std::ifstream file("scores.txt");
            if (!file.is_open()) return;

            const int BUFFER_SIZE = 1000;
            HighScoreEntry tempBuffer[BUFFER_SIZE];
            int count = 0;
            std::string line;

            while (std::getline(file, line) && count < BUFFER_SIZE)
            {
                size_t namePos = line.find("Name: ");
                size_t barPos = line.find(" | Score: ");
                size_t timePos = line.find(" | Time: ");

                if (namePos != std::string::npos && barPos != std::string::npos)
                {
                    try
                    {
                        std::string n = line.substr(namePos + 6, barPos - (namePos + 6));
                        std::string s_str;
                        if (timePos != std::string::npos) s_str = line.substr(barPos + 9, timePos - (barPos + 9));
                        else s_str = line.substr(barPos + 9);

                        int s = std::stoi(s_str);
                        std::string t = "N/A";
                        if (timePos != std::string::npos) t = line.substr(timePos + 8);

                        tempBuffer[count].name = n;
                        tempBuffer[count].score = s;
                        tempBuffer[count].timePlayed = t;
                        count++;
                    }
                    catch (...) { continue; }
                }
            }
            file.close();

            for (int i = 0; i < count - 1; i++)
            {
                for (int j = 0; j < count - i - 1; j++)
                {
                    if (tempBuffer[j].score < tempBuffer[j + 1].score)
                    {
                        HighScoreEntry temp = tempBuffer[j];
                        tempBuffer[j] = tempBuffer[j + 1];
                        tempBuffer[j + 1] = temp;
                    }
                }
            }

            scoresLoadedCount = (count < MAX_HIGH_SCORES) ? count : MAX_HIGH_SCORES;
            for (int i = 0; i < scoresLoadedCount; i++) topScores[i] = tempBuffer[i];
            scrollOffset = 0.0f;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error loading scores: " << e.what() << std::endl;
        }
    }

public:


    Game()
    {
        std::srand(static_cast<unsigned>(std::time(0)));
        window.create(sf::VideoMode(800, 600), "Galaxy Wars");
        window.setFramerateLimit(60);

        try
        {
            if (!menuBgTexture.loadFromFile("assets/menu_bg.jpg"))
            {
                std::cerr << "Warning: Background image not found." << std::endl;
            }
            if (!font.loadFromFile("assets/Fonts/Arial.ttf"))
            {
                throw GameException("CRITICAL: Failed to load font assets/Fonts/Arial.ttf");
            }
        }
        catch (const GameException& e)
        {
            std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        }

        slotMenu = new SlotMenu((float)window.getSize().x, (float)window.getSize().y);
        menuBgSprite.setTexture(menuBgTexture);
        float scaleX = 800.0f / menuBgTexture.getSize().x;
        float scaleY = 600.0f / menuBgTexture.getSize().y;
        menuBgSprite.setScale(scaleX, scaleY);

        menu = new Menu((float)window.getSize().x, (float)window.getSize().y);
        levelMenu = new LevelMenu((float)window.getSize().x, (float)window.getSize().y);
        pauseMenu = new PauseMenu((float)window.getSize().x, (float)window.getSize().y);
        gameOverScreen = new GameOverScreen((float)window.getSize().x, (float)window.getSize().y);
        player = new Player();
        dashboard = new Dashboard((float)window.getSize().x, (float)window.getSize().y);
        soundManager = new SoundManager();

        namePromptText.setFont(font);
        namePromptText.setString("ENTER YOUR NAME:");
        namePromptText.setCharacterSize(40);
        namePromptText.setFillColor(sf::Color::Yellow);
        sf::FloatRect npBounds = namePromptText.getLocalBounds();
        namePromptText.setOrigin(npBounds.width / 2, npBounds.height / 2);
        namePromptText.setPosition(400, 200);

        playerName = "";
        nameInputText.setFont(font);
        nameInputText.setString("_");
        nameInputText.setCharacterSize(40);
        nameInputText.setFillColor(sf::Color::White);
        nameInputText.setPosition(300, 300);

        welcomeText.setFont(font);
        welcomeText.setCharacterSize(30);
        welcomeText.setFillColor(sf::Color::Cyan);

        scoreDisplayTitle.setFont(font);
        scoreDisplayTitle.setString("TOP COMMANDERS");
        scoreDisplayTitle.setCharacterSize(40);
        scoreDisplayTitle.setFillColor(sf::Color::Red);
        scoreDisplayTitle.setStyle(sf::Text::Bold);
        scoreDisplayTitle.setOutlineColor(sf::Color::Black);
        scoreDisplayTitle.setOutlineThickness(2);
        sf::FloatRect sdBounds = scoreDisplayTitle.getLocalBounds();
        scoreDisplayTitle.setOrigin(sdBounds.width / 2, sdBounds.height / 2);
        scoreDisplayTitle.setPosition(400, 50);

        scoreColumnHeader.setFont(font);
        scoreColumnHeader.setCharacterSize(22);
        scoreColumnHeader.setFillColor(sf::Color::Yellow);
        scoreColumnHeader.setStyle(sf::Text::Bold);

        scoreListText.setFont(font);
        scoreListText.setCharacterSize(20);
        scoreListText.setFillColor(sf::Color::White);

        scoreBoardBackground.setSize(sf::Vector2f(750.f, 480.f));
        scoreBoardBackground.setFillColor(sf::Color(0, 0, 0, 220));
        scoreBoardBackground.setOutlineColor(sf::Color::Cyan);
        scoreBoardBackground.setOutlineThickness(2);
        scoreBoardBackground.setOrigin(375.f, 240.f);
        scoreBoardBackground.setPosition(400.f, 300.f);

        soundManager->startMenuMusic();

        for (int i = 0; i < MAX_ENEMIES; i++) enemies[i] = new Enemy();
        for (int i = 0; i < MAX_BULLETS; i++) bullets[i] = new Bullet();
        for (int i = 0; i < MAX_MISSILES; i++) missiles[i] = new Missile();
        for (int i = 0; i < MAX_ASTEROIDS; i++) asteroids[i] = new Asteroid();

        boss = new Boss();

        gameState = 0;
        currentLevel = 1;
        levelKills = 0;
        isGameFrozen = false;
        scoresLoadedCount = 0;
        scrollOffset = 0;
    }

    ~Game()
    {

        delete menu; delete levelMenu; delete pauseMenu; delete gameOverScreen;
        delete player; delete dashboard; delete soundManager; delete slotMenu;
        for (int i = 0; i < MAX_ENEMIES; i++) delete enemies[i];
        for (int i = 0; i < MAX_BULLETS; i++) delete bullets[i];
        for (int i = 0; i < MAX_MISSILES; i++) delete missiles[i];
        for (int i = 0; i < MAX_ASTEROIDS; i++) delete asteroids[i];
        delete boss;
    }

    void run()

    {
        while (window.isOpen())
        {
            processEvents();
            update();
            render();
        }
    }

private:


    void processEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (gameState == 0)
            {
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Up) menu->moveUp();
                    if (event.key.code == sf::Keyboard::Down) menu->moveDown();
                    if (event.key.code == sf::Keyboard::Return) {
                        int x = menu->getPressedItem();
                        if (x == 0) { currentLevel = 1; gameState = 6; playerName = ""; }
                        if (x == 1) { gameState = 8; slotMenu->setMode(false); }
                        if (x == 2) gameState = 5;
                        if (x == 3) { gameState = 2; loadHighScores(); }
                        if (x == 4) window.close();
                    }
                }
            }
            else if (gameState == 2)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Return) gameState = 0;
                    if (event.key.code == sf::Keyboard::Down) scrollOffset += 30.0f;
                    if (event.key.code == sf::Keyboard::Up) { scrollOffset -= 30.0f; if (scrollOffset < 0) scrollOffset = 0; }
                }
            }
            else if (gameState == 5)
            {
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Up) levelMenu->moveUp();
                    if (event.key.code == sf::Keyboard::Down) levelMenu->moveDown();
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        int choice = levelMenu->getPressedItem();
                        if (choice == 0) currentLevel = 1;
                        if (choice == 1) currentLevel = 2;
                        if (choice == 2) currentLevel = 3;
                        gameState = 6; playerName = "";
                    }
                    if (event.key.code == sf::Keyboard::Escape) gameState = 0;
                }
            }
            else if (gameState == 6)
            {
                if (event.type == sf::Event::TextEntered)
                {
                    if (event.text.unicode == 8) { if (!playerName.empty()) playerName.pop_back(); }
                    else if (event.text.unicode < 128 && event.text.unicode != 13) playerName += static_cast<char>(event.text.unicode);
                    nameInputText.setString(playerName + "_");
                    sf::FloatRect iBounds = nameInputText.getLocalBounds();
                    nameInputText.setOrigin(iBounds.width / 2, iBounds.height / 2);
                    nameInputText.setPosition(400, 300);
                }
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        if (playerName.empty()) playerName = "Commander";
                        gameState = 7;
                        fullWelcomeMessage = "Welcome " + playerName + ",\nGalaxy War awaits!";
                        currentWelcomeDisplay = ""; charIndex = 0;
                        welcomeStateClock.restart(); typewriterClock.restart();
                    }
                }
            }
            else if (gameState == 7)
            {
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Return) startGame(currentLevel);
                }
            }
            else if (gameState == 1)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::P)
                    {
                        gameState = 3; dashboard->startPause();
                        soundManager->pauseGameMusic(); soundManager->startMenuMusic();
                    }
                    if (event.key.code == sf::Keyboard::Space) {
                        fireMissile(); soundManager->playPlayerFire();
                    }
                }
            }
            else if (gameState == 8)
            {
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Up) slotMenu->moveUp();
                    if (event.key.code == sf::Keyboard::Down) slotMenu->moveDown();
                    if (event.key.code == sf::Keyboard::Escape) gameState = 0;
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        int slot = slotMenu->getPressedItem();
                        if (loadGame(slot))
                        {
                            gameState = 1; soundManager->stopMenuMusic(); soundManager->startGameMusic();
                        }
                    }
                }
            }
            else if (gameState == 9)
            {
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Up) slotMenu->moveUp();
                    if (event.key.code == sf::Keyboard::Down) slotMenu->moveDown();
                    if (event.key.code == sf::Keyboard::Escape) gameState = 3;
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        int slot = slotMenu->getPressedItem();
                        saveGame(slot);
                        gameState = 1; dashboard->endPause();
                        soundManager->stopMenuMusic(); soundManager->resumeGameMusic();
                    }
                }
            }
            else if (gameState == 3)
            {
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Up) pauseMenu->moveUp();
                    if (event.key.code == sf::Keyboard::Down) pauseMenu->moveDown();
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        int choice = pauseMenu->getPressedItem();
                        if (choice == 0) { gameState = 1; dashboard->endPause(); soundManager->stopMenuMusic(); soundManager->resumeGameMusic(); }
                        if (choice == 1) { gameState = 9; slotMenu->setMode(true); }
                        if (choice == 2) startGame(1);
                        if (choice == 3) { gameState = 0; soundManager->stopGameMusic(); soundManager->startMenuMusic(); }
                    }
                }
            }
        }
    }

    void update()
    {
        if (gameState == 7)
        {
            if (typewriterClock.getElapsedTime().asSeconds() > 0.1f && charIndex < fullWelcomeMessage.length())
            {
                currentWelcomeDisplay += fullWelcomeMessage[charIndex]; charIndex++;
                welcomeText.setString(currentWelcomeDisplay);
                sf::FloatRect bounds = welcomeText.getLocalBounds();
                welcomeText.setOrigin(bounds.width / 2, bounds.height / 2);
                welcomeText.setPosition(400, 300);
                typewriterClock.restart();
            }
            if (welcomeStateClock.getElapsedTime().asSeconds() > 5.0f) startGame(currentLevel);
        }
        else if (gameState == 1)
        {
            player->update();
            dashboard->update();

            if (isGameFrozen)
            {
                if (freezeTimer.getElapsedTime().asSeconds() > 10.0f) isGameFrozen = false;
            }

            if (!isGameFrozen)
            {
                if (enemySpawnClock.getElapsedTime().asSeconds() > 1.5f)
                {
                    spawnEnemy(); enemySpawnClock.restart();
                }
                if (asteroidSpawnClock.getElapsedTime().asSeconds() > 3.0f)
                {
                    spawnAsteroid(); asteroidSpawnClock.restart();
                }
            }

            if (powerupSpawnClock.getElapsedTime().asSeconds() > 15.0f) { spawnPowerUp(); powerupSpawnClock.restart(); }

            if (currentLevel == 3)
            {
                if (!boss->isActive() && !boss->isDefeated())
                {
                    boss->spawn();
                }
                if (boss->isActive())
                {
                    if (!isGameFrozen)
                    {
                        boss->update();
                        if (boss->shouldFire())
                        {
                            spawnBullet(boss->getBounds().left + boss->getBounds().width / 2,
                                boss->getBounds().top + boss->getBounds().height);
                            soundManager->playEnemyFire();
                        }
                    }
                    boss->updateColor();
                    if (boss->getBounds().intersects(player->getBounds()))
                    {
                        if (!player->hasShield())
                        {
                            dashboard->loseLife(); player->resetPosition(); soundManager->playShipExplosion();
                            if (dashboard->getLives() <= 0) handleGameOver();
                        }
                    }
                }
            }

            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i]->isActive())
                {
                    if (!isGameFrozen)
                    {
                        enemies[i]->update();
                        if (rand() % 200 == 0)
                        {
                            spawnBullet(enemies[i]->getBounds().left + enemies[i]->getBounds().width / 2,
                                enemies[i]->getBounds().top + enemies[i]->getBounds().height);
                            soundManager->playEnemyFire();
                        }
                    }
                    if (player->getBounds().intersects(enemies[i]->getBounds()))
                    {
                        if (!player->hasShield())
                        {
                            enemies[i]->deactivate(); dashboard->loseLife(); player->resetPosition(); soundManager->playShipExplosion();
                            isGameFrozen = false;
                            if (dashboard->getLives() <= 0) handleGameOver();
                        }
                        else
                        {
                            enemies[i]->deactivate(); dashboard->addScore(100); soundManager->playShipExplosion();
                        }
                    }
                }
            }

            for (int i = 0; i < MAX_ASTEROIDS; i++)
            {
                if (asteroids[i]->isActive())
                {
                    if (!isGameFrozen) asteroids[i]->update();
                    if (player->getBounds().intersects(asteroids[i]->getBounds()))
                    {
                        if (!player->hasShield())
                        {
                            asteroids[i]->deactivate(); dashboard->loseLife(); player->resetPosition(); soundManager->playShipExplosion();
                            isGameFrozen = false;
                            if (dashboard->getLives() <= 0) handleGameOver();
                        }
                        else
                        {
                            asteroids[i]->deactivate(); soundManager->playShipExplosion();
                        }
                    }
                }
            }

            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i]->isActive())
                {
                    if (!isGameFrozen) bullets[i]->update();
                    if (bullets[i]->getBounds().intersects(player->getBounds()))
                    {
                        if (!player->hasShield())
                        {
                            bullets[i]->deactivate(); dashboard->loseLife(); player->resetPosition(); soundManager->playShipExplosion();
                            isGameFrozen = false;
                            if (dashboard->getLives() <= 0) handleGameOver();
                        }
                        else
                        {
                            bullets[i]->deactivate();
                        }
                    }
                }
            }

            for (int i = 0; i < powerups.getSize(); i++)
            {
                PowerUp* p = powerups.get(i);
                if (p->isActive())
                {
                    p->update();
                    if (player->getBounds().intersects(p->getBounds()))
                    {
                        (*player) + (*p);
                        int type = p->getType();
                        if (type == 1) dashboard->addLife();
                        else if (type == 2) dashboard->addScore(100);
                        else if (type == 3) { isGameFrozen = true; freezeTimer.restart(); }
                        dashboard->addScore(50);
                        dashboard->addPowerupCount();
                        p->deactivate();
                        soundManager->playPowerupSound();
                    }
                }
            }

            for (int i = 0; i < MAX_MISSILES; i++)
            {
                if (missiles[i]->isActive())
                {
                    missiles[i]->update();
                    bool missileDestroyed = false;

                    if (boss->isActive() && missiles[i]->getBounds().intersects(boss->getBounds()))
                    {
                        missiles[i]->deactivate(); boss->takeDamage(); soundManager->playBulletCollision(); missileDestroyed = true;
                        if (boss->getHealth() <= 0)
                        {
                            boss->kill(); dashboard->addScore(5000); soundManager->playShipExplosion(); handleVictory();
                        }
                    }
                    if (missileDestroyed) continue;

                    for (int k = 0; k < MAX_BULLETS; k++) {
                        if (bullets[k]->isActive() && missiles[i]->getBounds().intersects(bullets[k]->getBounds()))
                        {
                            missiles[i]->deactivate(); bullets[k]->deactivate(); missileDestroyed = true; soundManager->playBulletCollision(); break;
                        }
                    }
                    if (missileDestroyed) continue;

                    for (int j = 0; j < MAX_ENEMIES; j++)
                    {
                        if (enemies[j]->isActive() && missiles[i]->getBounds().intersects(enemies[j]->getBounds()))
                        {
                            missiles[i]->deactivate(); enemies[j]->deactivate(); missileDestroyed = true;
                            dashboard->addScore(100); levelKills++; soundManager->playShipExplosion(); break;
                        }
                    }
                    if (missileDestroyed) continue;

                    for (int a = 0; a < MAX_ASTEROIDS; a++)
                    {
                        if (asteroids[a]->isActive() && missiles[i]->getBounds().intersects(asteroids[a]->getBounds()))
                        {
                            missiles[i]->deactivate(); asteroids[a]->deactivate(); missileDestroyed = true;
                            dashboard->addScore(50); soundManager->playShipExplosion(); break;
                        }
                    }
                }
            }

            if (currentLevel == 1 && levelKills >= 5) { currentLevel = 2; levelKills = 0; dashboard->updateLevel(2); }
            else if (currentLevel == 2 && levelKills >= 10)
            {
                currentLevel = 3; levelKills = 0; dashboard->updateLevel(3);
            }
        }
        else if (gameState == 4)
        {
            if (gameOverClock.getElapsedTime().asSeconds() > 4.0f) { gameState = 0; soundManager->startMenuMusic(); }
        }
    }

    void handleGameOver()
    {


        gameState = 4;
        gameOverScreen->setVictory(false);
        gameOverScreen->setFinalScore(dashboard->getScore());
        gameOverClock.restart();
        soundManager->stopGameMusic();
        soundManager->playGameOverSound();
        saveScoreToFile();
    }

    void handleVictory()
    {


        gameState = 4;
        gameOverScreen->setVictory(true);
        gameOverScreen->setFinalScore(dashboard->getScore());
        gameOverClock.restart();
        soundManager->stopGameMusic();
        soundManager->playVictorySound();
        saveScoreToFile();
    }

    void fireMissile()
    {


        for (int i = 0; i < MAX_MISSILES; i++)
        {
            if (!missiles[i]->isActive())
            {
                sf::FloatRect pBounds = player->getBounds();
                missiles[i]->fire(pBounds.left + pBounds.width / 2, pBounds.top);
                break;
            }
        }
    }

    void spawnBullet(float x, float y)
    {
        for (int i = 0; i < MAX_BULLETS; i++) { if (!bullets[i]->isActive()) { bullets[i]->spawn(x, y); break; } }
    }

    void spawnEnemy()
    {
        int index = -1;
        for (int i = 0; i < MAX_ENEMIES; i++) { if (!enemies[i]->isActive()) { index = i; break; } }
        if (index != -1)
        {
            float randomX = static_cast<float>(rand() % 700 + 50);
            float speed = (currentLevel == 1) ? 1.0f : 1.5f;
            enemies[index]->spawn(randomX, -50.f, speed, currentLevel);
        }
    }

    void spawnAsteroid()
    {


        int index = -1;
        for (int i = 0; i < MAX_ASTEROIDS; i++) { if (!asteroids[i]->isActive()) { index = i; break; } }
        if (index != -1)
        {
            float speed = (currentLevel == 1) ? 1.0f : 1.5f;
            asteroids[index]->spawn(static_cast<float>(rand() % 700 + 50), -50.f, speed);
        }
    }

    void spawnPowerUp()
    {


        PowerUp* p = powerups.getAvailable();
        if (p != nullptr)
        {
            float randomX = static_cast<float>(rand() % 700 + 50);
            p->spawn(randomX, -50.f, rand() % 4);
        }
    }

    void resetEntities()
    {


        for (int i = 0; i < MAX_ENEMIES; i++) enemies[i]->deactivate();
        for (int i = 0; i < MAX_BULLETS; i++) bullets[i]->deactivate();
        for (int i = 0; i < MAX_MISSILES; i++) missiles[i]->deactivate();
        for (int i = 0; i < MAX_ASTEROIDS; i++) asteroids[i]->deactivate();
        for (int i = 0; i < powerups.getSize(); i++) powerups.get(i)->deactivate();
        boss->reset();
    }

    void render()
    {


        window.clear(sf::Color::Black);

        if (gameState == 0)
        {
            window.draw(menuBgSprite);
            menu->draw(window);
        }
        else if (gameState == 2)
        {
            window.draw(menuBgSprite);
            window.draw(scoreBoardBackground);
            window.draw(scoreDisplayTitle);

            scoreColumnHeader.setString("RANK"); scoreColumnHeader.setPosition(100, 100); window.draw(scoreColumnHeader);
            scoreColumnHeader.setString("NAME"); scoreColumnHeader.setPosition(250, 100); window.draw(scoreColumnHeader);
            scoreColumnHeader.setString("SCORE"); scoreColumnHeader.setPosition(500, 100); window.draw(scoreColumnHeader);
            scoreColumnHeader.setString("TIME"); scoreColumnHeader.setPosition(650, 100); window.draw(scoreColumnHeader);

            float startY = 140.0f;
            float gap = 35.0f;


            for (int i = 0; i < scoresLoadedCount; i++)
            {
                float yPos = startY + (i * gap) - scrollOffset;

                if (yPos > 120 && yPos < 520)
                {
                    scoreListText.setFillColor(sf::Color::White);
                    if (i == 0) scoreListText.setFillColor(sf::Color::Yellow);

                    scoreListText.setString(std::to_string(i + 1));
                    scoreListText.setPosition(100, yPos);
                    window.draw(scoreListText);

                    scoreListText.setString(topScores[i].name);
                    scoreListText.setPosition(250, yPos);
                    window.draw(scoreListText);

                    scoreListText.setString(std::to_string(topScores[i].score));
                    scoreListText.setPosition(500, yPos);
                    window.draw(scoreListText);

                    scoreListText.setString(topScores[i].timePlayed);
                    scoreListText.setPosition(650, yPos);
                    window.draw(scoreListText);
                }
            }
        }
        else if (gameState == 5)
        {
            window.draw(menuBgSprite);
            levelMenu->draw(window);
        }
        else if (gameState == 6)
        {
            window.draw(namePromptText);
            window.draw(nameInputText);
        }
        else if (gameState == 7)
        {
            window.draw(welcomeText);
        }
        else if (gameState == 8 || gameState == 9)
        {
            window.draw(menuBgSprite);
            slotMenu->draw(window);
        }
        else if (gameState == 1 || gameState == 3)
        {


            sf::RectangleShape playground(sf::Vector2f(800.f, 480.f));
            playground.setFillColor(sf::Color::Black);
            window.draw(playground);

            if (isGameFrozen)

            {
                sf::RectangleShape freezeOverlay(sf::Vector2f(800.f, 480.f));
                freezeOverlay.setFillColor(sf::Color(0, 255, 255, 50));
                window.draw(freezeOverlay);
            }

            sf::RectangleShape dashboardBg(sf::Vector2f(800.f, 120.f));
            dashboardBg.setPosition(0.f, 480.f); dashboardBg.setFillColor(sf::Color(0, 0, 128));
            window.draw(dashboardBg);

            player->draw(window);
            for (int i = 0; i < MAX_ENEMIES; i++) enemies[i]->draw(window);
            for (int i = 0; i < MAX_BULLETS; i++) bullets[i]->draw(window);
            for (int i = 0; i < MAX_MISSILES; i++) missiles[i]->draw(window);
            for (int i = 0; i < MAX_ASTEROIDS; i++) asteroids[i]->draw(window);

            for (int i = 0; i < powerups.getSize(); i++)
            {
                powerups.get(i)->draw(window);
            }

            boss->draw(window);



            dashboard->draw(window);
            if (gameState == 3) pauseMenu->draw(window);
        }


        else if (gameState == 4) gameOverScreen->draw(window);

        window.display();
    }
};