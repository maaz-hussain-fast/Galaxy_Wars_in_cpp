// Galaxy Wars by Maaz & Danyal

#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

class SoundManager 
{

private:
    
    sf::SoundBuffer playerFireBuffer;
    sf::SoundBuffer enemyFireBuffer;
    sf::SoundBuffer shipExplosionBuffer;
    sf::SoundBuffer bulletCollisionBuffer;
    sf::SoundBuffer gameOverBuffer;     
    sf::SoundBuffer victoryBuffer;
    sf::SoundBuffer powerupBuffer;
    sf::Sound powerupSound;


    sf::Sound playerFireSound;
    sf::Sound enemyFireSound;
    sf::Sound shipExplosionSound;
    sf::Sound bulletCollisionSound;
    sf::Sound gameOverSound;            
    sf::Sound victorySound;       

    
    sf::Music menuMusic; 
    sf::Music gameMusic; 

public:


    SoundManager() 
    {
        loadSFX();
        loadMusic();
    }

    void loadSFX() {
        
        if (!playerFireBuffer.loadFromFile("assets/sfx/laser8.wav")) std::cout << "Error: laser8.wav missing" << std::endl;
        playerFireSound.setBuffer(playerFireBuffer);
        playerFireSound.setVolume(40.f);

        
        if (!enemyFireBuffer.loadFromFile("assets/sfx/laser1.wav")) std::cout << "Error: laser1.wav missing" << std::endl;
        enemyFireSound.setBuffer(enemyFireBuffer);
        enemyFireSound.setVolume(30.f);

        
        if (!shipExplosionBuffer.loadFromFile("assets/sfx/spaceship_explosion.flac")) std::cout << "Error: spaceship_explosion.flac missing" << std::endl;
        shipExplosionSound.setBuffer(shipExplosionBuffer);
        shipExplosionSound.setVolume(70.f);

        
        if (!bulletCollisionBuffer.loadFromFile("assets/sfx/bullets_collision.wav")) std::cout << "Error: bullets_collision.wav missing" << std::endl;
        bulletCollisionSound.setBuffer(bulletCollisionBuffer);
        bulletCollisionSound.setVolume(50.f);

        
        if (!gameOverBuffer.loadFromFile("assets/sfx/gameover.wav")) {
            
            if (!gameOverBuffer.loadFromFile("assets/sfx/gameover.wav")) std::cout << "Error: gameover.wav missing" << std::endl;
        }
        gameOverSound.setBuffer(gameOverBuffer);
        gameOverSound.setVolume(80.f);



        if (!victoryBuffer.loadFromFile("assets/sfx/victory.mp3")) {
            std::cout << "Error loading victory.mp3" << std::endl;
        }
        victorySound.setBuffer(victoryBuffer);

        if (!powerupBuffer.loadFromFile("assets/sfx/powerup.wav")) {
            
        }
        powerupSound.setBuffer(powerupBuffer);
        powerupSound.setVolume(60.f);
    }

    void loadMusic() {
        
        if (!menuMusic.openFromFile("assets/sfx/menu_chill.wav")) {
            std::cout << "Error: menu_chill.wav missing" << std::endl;
        }
        menuMusic.setLoop(true);
        menuMusic.setVolume(40.f);

        
        if (!gameMusic.openFromFile("assets/sfx/War.mp3")) {
            std::cout << "Error: War.mp3 missing" << std::endl;
        }
        gameMusic.setLoop(true);
        gameMusic.setVolume(30.f);
    }

    void playVictorySound() {
        victorySound.play();
    }

    
    void playPlayerFire() {

        float pitch = 0.9f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2f));
        playerFireSound.setPitch(pitch);
        playerFireSound.play();
    }

    void playEnemyFire() {

        float pitch = 0.8f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2f));
        enemyFireSound.setPitch(pitch);
        enemyFireSound.play();
    }

    void playShipExplosion() {

        shipExplosionSound.setPitch(1.0f);
        shipExplosionSound.play();
    }

    void playBulletCollision() {

        float pitch = 0.9f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2f));
        bulletCollisionSound.setPitch(pitch);
        bulletCollisionSound.play();
    }

    void playGameOverSound() {

        gameOverSound.play();
    }

    
    void startMenuMusic() {

        if (menuMusic.getStatus() != sf::Music::Playing)
            menuMusic.play();
    }
    void stopMenuMusic()
    {
        menuMusic.stop();
    }

    void startGameMusic() 
    {
        
        gameMusic.stop();
        gameMusic.play();
    }
    void resumeGameMusic() {
        
        if (gameMusic.getStatus() != sf::Music::Playing)
            gameMusic.play();
    }
    void pauseGameMusic() 
    {

        gameMusic.pause();
    }
    void stopGameMusic() 
    {

        gameMusic.stop();
    }
    void playPowerupSound() 
    {

        powerupSound.play();
    }
};