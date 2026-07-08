#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Ships.h"
#include "GameObjects.h"
#include "Managers.h"
#include "GameObjectArray.h"

using namespace std;

namespace GalaxyWars {

    enum GameState {
        SPLASH_SCREEN,
        MAIN_MENU,
        NAME_INPUT,
        INSTRUCTIONS,
        LEADERBOARD,
        PLAYING,
        PAUSED,
        LEVEL_TRANSITION,
        GAME_OVER,
        GAME_COMPLETE
    };

    class GameEngine {
        sf::RenderWindow window;
        Player* player;
        GameObjectArray<Projectile> bullets;
        GameObjectArray<Enemy> enemies;
        GameObjectArray<Asteroid> asteroids;
        GameObjectArray<PowerUp> powerups;
        GameObjectArray<EnemyBullet> enemyBullets;

        sf::Texture splashTexture;
        sf::Texture menuTexture;
        sf::Texture instructionsTexture;
        sf::Texture leaderboardTexture;
        sf::Texture pauseTexture;
        sf::Texture gameoverTexture;
        sf::Texture gamecompleteTexture;
        sf::Texture level1Texture;
        sf::Texture level2Texture;
        sf::Texture bgTexture;

        sf::Sprite splashSprite;
        sf::Sprite menuSprite;
        sf::Sprite instructionsSprite;
        sf::Sprite leaderboardSprite;
        sf::Sprite pauseSprite;
        sf::Sprite gameoverSprite;
        sf::Sprite gamecompleteSprite;
        sf::Sprite level1Sprite;
        sf::Sprite level2Sprite;
        sf::Sprite bgSprite;

        sf::Font font;
        sf::Text hudtext;
        sf::Clock frameclock;
        sf::Clock splashClock;
        sf::Clock spawnclock;
        sf::Clock powerupclock;
        sf::Clock levelclock;
        sf::Clock levelTransitionClock;
        sf::Clock level3TimerClock;

        sf::Music menuMusic;
        sf::Music gameplayMusic;
        sf::Music gameoverMusic;
        sf::Music victoryMusic;

        int score;
        bool running;
        GameState state;
        GameState previousMusicState;
        LevelManager levelmanager;
        bool bossSpawned;
        int splashAlpha;
        bool fadingOut;
        int previousLevel;
        bool level1Shown;
        bool level2Shown;
        int transitionLevel;
        float level3Duration;
        bool level3Started;
        bool justEnteredNameInput;
        string playerName;

    public:
        GameEngine() : window(sf::VideoMode(800, 600), "Galaxy Wars - OOP Project"), player(nullptr), score(0), running(true), state(SPLASH_SCREEN), previousMusicState(SPLASH_SCREEN), bossSpawned(false), splashAlpha(255), fadingOut(false), previousLevel(1), level1Shown(false), level2Shown(false), transitionLevel(1), level3Duration(120.0f), level3Started(false), playerName(""), justEnteredNameInput(false) {
            window.setFramerateLimit(60);
            loadMusic();

            if (!splashTexture.loadFromFile("Sprites/firstpage.jpg")) {
                cout << "Splash texture not found" << endl;
            }
            splashSprite.setTexture(splashTexture);
            float scaleX = 800.0f / splashTexture.getSize().x;
            float scaleY = 600.0f / splashTexture.getSize().y;
            splashSprite.setScale(scaleX, scaleY);

            if (!menuTexture.loadFromFile("Sprites/menupage.jpg")) {
                cout << "Menu texture not found" << endl;
            }
            menuSprite.setTexture(menuTexture);
            scaleX = 800.0f / menuTexture.getSize().x;
            scaleY = 600.0f / menuTexture.getSize().y;
            menuSprite.setScale(scaleX, scaleY);

            if (!instructionsTexture.loadFromFile("Sprites/instructionspage.jpeg")) {
                cout << "Instructions texture not found" << endl;
            }
            instructionsSprite.setTexture(instructionsTexture);
            scaleX = 800.0f / instructionsTexture.getSize().x;
            scaleY = 600.0f / instructionsTexture.getSize().y;
            instructionsSprite.setScale(scaleX, scaleY);

            if (!leaderboardTexture.loadFromFile("Sprites/leaderboardpage.jpeg")) {
                cout << "Leaderboard texture not found" << endl;
            }
            leaderboardSprite.setTexture(leaderboardTexture);
            scaleX = 800.0f / leaderboardTexture.getSize().x;
            scaleY = 600.0f / leaderboardTexture.getSize().y;
            leaderboardSprite.setScale(scaleX, scaleY);

            if (!pauseTexture.loadFromFile("Sprites/pausemenu.png")) {
                cout << "Pause menu texture not found" << endl;
            }
            pauseSprite.setTexture(pauseTexture);
            scaleX = 800.0f / pauseTexture.getSize().x;
            scaleY = 600.0f / pauseTexture.getSize().y;
            pauseSprite.setScale(scaleX, scaleY);

            if (!gameoverTexture.loadFromFile("Sprites/gameover.png")) {
                cout << "Game over texture not found" << endl;
            }
            gameoverSprite.setTexture(gameoverTexture);
            scaleX = 800.0f / gameoverTexture.getSize().x;
            scaleY = 600.0f / gameoverTexture.getSize().y;
            gameoverSprite.setScale(scaleX, scaleY);

            if (!gamecompleteTexture.loadFromFile("Sprites/gamecomplete.png")) {
                cout << "Game complete texture not found" << endl;
            }
            gamecompleteSprite.setTexture(gamecompleteTexture);
            scaleX = 800.0f / gamecompleteTexture.getSize().x;
            scaleY = 600.0f / gamecompleteTexture.getSize().y;
            gamecompleteSprite.setScale(scaleX, scaleY);

            if (!level1Texture.loadFromFile("Sprites/level1.png")) {
                cout << "Level 1 texture not found" << endl;
            }
            level1Sprite.setTexture(level1Texture);
            scaleX = 800.0f / level1Texture.getSize().x;
            scaleY = 600.0f / level1Texture.getSize().y;
            level1Sprite.setScale(scaleX, scaleY);

            if (!level2Texture.loadFromFile("Sprites/level2.png")) {
                cout << "Level 2 texture not found" << endl;
            }
            level2Sprite.setTexture(level2Texture);
            scaleX = 800.0f / level2Texture.getSize().x;
            scaleY = 600.0f / level2Texture.getSize().y;
            level2Sprite.setScale(scaleX, scaleY);

            if (!bgTexture.loadFromFile("Sprites/starfield_background.jpg")) {
                cout << "Background texture not found" << endl;
            }
            bgSprite.setTexture(bgTexture);
            scaleX = 800.0f / bgTexture.getSize().x;
            scaleY = 600.0f / bgTexture.getSize().y;
            bgSprite.setScale(scaleX, scaleY);

            if (!font.loadFromFile("arial/Skranji-Regular.ttf")) {
                cout << "Font not found" << endl;
            }
            hudtext.setFont(font);
            hudtext.setCharacterSize(20);
            hudtext.setFillColor(sf::Color::White);

            player = new Player(400.0f, 500.0f, &bullets);
        }

        ~GameEngine() {
            delete player;
        }

        void loadMusic()
        {
            // try loading menu music
            if (!menuMusic.openFromFile("Music/menu.ogg")) {
                cout << "Menu music not found - trying .wav" << endl;
                if (!menuMusic.openFromFile("Music/menu.wav")) {
                    cout << "Menu music not found!" << endl;
                }
            }
            menuMusic.setLoop(true);
            menuMusic.setVolume(50);

            if (!gameplayMusic.openFromFile("Music/gameplay.ogg")) {
                cout << "Gameplay music not found - trying .wav" << endl;
                if (!gameplayMusic.openFromFile("Music/gameplay.wav")) {
                    cout << "Gameplay music not found!" << endl;
                }
            }
            gameplayMusic.setLoop(true);
            gameplayMusic.setVolume(40);

            if (!gameoverMusic.openFromFile("Music/gameover.ogg")) {
                cout << "Game over music not found - trying .wav" << endl;
                if (!gameoverMusic.openFromFile("Music/gameover.wav")) {
                    cout << "Game over music not found!" << endl;
                }
            }
            gameoverMusic.setLoop(false);
            gameoverMusic.setVolume(60);

            if (!victoryMusic.openFromFile("Music/victory.ogg")) {
                cout << "Victory music not found - trying .wav" << endl;
                if (!victoryMusic.openFromFile("Music/victory.wav")) {
                    cout << "Victory music not found!" << endl;
                }
            }
            victoryMusic.setLoop(false);
            victoryMusic.setVolume(60);
        }

        void updateMusic()
        {
            if (state == previousMusicState)
                return;

            menuMusic.stop();
            gameplayMusic.stop();
            gameoverMusic.stop();
            victoryMusic.stop();

            if (state == MAIN_MENU || state == NAME_INPUT || state == INSTRUCTIONS || state == LEADERBOARD) {
                menuMusic.play();
            }
            else if (state == PLAYING || state == LEVEL_TRANSITION) {
                gameplayMusic.play();
            }
            else if (state == GAME_OVER) {
                gameoverMusic.play();
            }
            else if (state == GAME_COMPLETE) {
                victoryMusic.play();
            }
            else if (state == PAUSED) {
                if (gameplayMusic.getStatus() == sf::Music::Playing) {
                    gameplayMusic.pause();
                }
            }

            previousMusicState = state;
        }

        void run()
        {
            frameclock.restart();
            splashClock.restart();
            menuMusic.play();

            while (window.isOpen() && running) {
                float deltaTime = frameclock.restart().asSeconds();
                handleEvents();
                update(deltaTime);
                updateMusic();
                render();
            }
        }

        void handleEvents()
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    running = false;
                    window.close();
                }

                if (state == SPLASH_SCREEN) {
                    if (event.type == sf::Event::KeyPressed) {
                        state = MAIN_MENU;
                    }
                }
                else if (state == MAIN_MENU) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Num1) {
                            state = NAME_INPUT;
                            playerName = "";
                            justEnteredNameInput = true;
                        }
                        else if (event.key.code == sf::Keyboard::Num2) {
                            state = LEADERBOARD;
                        }
                        else if (event.key.code == sf::Keyboard::Num3) {
                            state = INSTRUCTIONS;
                        }
                        else if (event.key.code == sf::Keyboard::Num4) {
                            running = false;
                            window.close();
                        }
                    }
                }
                else if (state == NAME_INPUT) {
                    if (justEnteredNameInput) {
                        justEnteredNameInput = false;
                        continue;
                    }

                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode == 8) {
                            if (!playerName.empty()) {
                                playerName.pop_back();
                            }
                        }
                        else if (event.text.unicode == 13) {
                            if (!playerName.empty()) {
                                state = PLAYING;
                                spawnclock.restart();
                                powerupclock.restart();
                                levelclock.restart();
                            }
                        }
                        else if (event.text.unicode < 128 && playerName.length() < 15) {
                            char ch = static_cast<char>(event.text.unicode);
                            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_') {
                                playerName += ch;
                            }
                        }
                    }

                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape) {
                            state = MAIN_MENU;
                            playerName = "";
                        }
                    }
                }
                else if (state == INSTRUCTIONS) {
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                        state = MAIN_MENU;
                    }
                }
                else if (state == LEADERBOARD) {
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                        state = MAIN_MENU;
                    }
                }
                else if (state == PLAYING) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::P) {
                            state = PAUSED;
                        }
                        else if (event.key.code == sf::Keyboard::F) {
                            player->toggleAutoFire();
                        }
                        else if (event.key.code == sf::Keyboard::Space) {
                            player->shoot();
                        }
                    }
                }
                else if (state == PAUSED) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Num1) {
                            state = PLAYING;
                            if (gameplayMusic.getStatus() == sf::Music::Paused) {
                                gameplayMusic.play();
                            }
                        }
                        else if (event.key.code == sf::Keyboard::Num2) {
                            state = MAIN_MENU;
                        }
                    }
                }
                else if (state == GAME_OVER) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape) {
                            state = MAIN_MENU;
                        }
                        else if (event.key.code == sf::Keyboard::X) {
                            running = false;
                            window.close();
                        }
                    }
                }
                else if (state == GAME_COMPLETE) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape) {
                            state = MAIN_MENU;
                        }
                        else if (event.key.code == sf::Keyboard::X) {
                            running = false;
                            window.close();
                        }
                    }
                }
            }
        }

        void update(float deltaTime)
        {
            if (state == SPLASH_SCREEN)
            {
                float elapsed = splashClock.getElapsedTime().asSeconds();

                if (elapsed > 4.0f && !fadingOut) {
                    fadingOut = true;
                }

                if (fadingOut) {
                    splashAlpha -= 5;
                    if (splashAlpha <= 0) {
                        splashAlpha = 0;
                        state = MAIN_MENU;
                    }
                    splashSprite.setColor(sf::Color(255, 255, 255, splashAlpha));
                }
            }
            else if (state == LEVEL_TRANSITION) {
                float elapsed = levelTransitionClock.getElapsedTime().asSeconds();
                float requiredTime = 5.0f;

                if (elapsed >= requiredTime) {
                    state = PLAYING;
                    if (transitionLevel == 1) {
                        level1Shown = true;
                    }
                    else if (transitionLevel == 2) {
                        level2Shown = true;
                    }
                }
            }
            else if (state == PLAYING) {
                player->update();
                player->tick(deltaTime);

                score = player->getScore();

                int currentLevel = levelmanager.getLevel();
                levelmanager.updateLevel(score);

                if (levelmanager.getLevel() == 2 && previousLevel == 1 && !level1Shown) {
                    state = LEVEL_TRANSITION;
                    transitionLevel = 1;
                    levelTransitionClock.restart();
                    level1Shown = true;
                    previousLevel = 2;
                }
                else if (levelmanager.getLevel() == 3 && previousLevel == 2 && !level2Shown) {
                    state = LEVEL_TRANSITION;
                    transitionLevel = 2;
                    levelTransitionClock.restart();
                    level2Shown = true;
                }

                if (levelmanager.getLevel() == 3 && !level3Started) {
                    level3Started = true;
                    level3TimerClock.restart();
                }

                if (level3Started && level3TimerClock.getElapsedTime().asSeconds() >= level3Duration)
                {
                    state = GAME_COMPLETE;

                    cout << endl << "Congratulations " << playerName << "! You completed the game!" << endl;
                    cout << "Final Score: " << score << endl;

                    float elapsed = levelclock.getElapsedTime().asSeconds();
                    try {
                        ScoreManager::saveScore(playerName, score, elapsed);
                        ScoreManager::saveBinaryScore(playerName, score, elapsed);
                    }
                    catch (GameException& err) {
                        cout << "Score save error: " << err.getMessage() << endl;
                    }
                }

                // boss spawn at level 3
                if (levelmanager.getLevel() == 3 && score > 2000 && !bossSpawned) {
                    enemies.add(new BossEnemy(400.0f, -80.0f));
                    bossSpawned = true;
                }

                if (spawnclock.getElapsedTime().asSeconds() > levelmanager.getSpawnInterval())
                {
                    float ex = static_cast<float>(rand() % 700);

                    if (levelmanager.getLevel() == 1) {
                        enemies.add(new Enemy(ex, -40.0f, levelmanager.getEnemySpeed()));
                    }
                    else if (levelmanager.getLevel() == 2) {
                        int enemyType = rand() % 2;
                        if (enemyType == 0) {
                            enemies.add(new Enemy(ex, -40.0f, levelmanager.getEnemySpeed()));
                        }
                        else {
                            enemies.add(new FastEnemy(ex, -40.0f));
                        }
                    }
                    else {
                        int enemyType = rand() % 3;
                        if (enemyType == 0) {
                            enemies.add(new Enemy(ex, -40.0f, levelmanager.getEnemySpeed()));
                        }
                        else if (enemyType == 1) {
                            enemies.add(new FastEnemy(ex, -40.0f));
                        }
                        else {
                            PatternEnemy* pEnemy = new PatternEnemy(ex, -40.0f, &enemyBullets);
                            enemies.add(pEnemy);
                        }
                    }

                    asteroids.add(new Asteroid(static_cast<float>(rand() % 700), -40.0f, 2.0f + levelmanager.getLevel()));
                    spawnclock.restart();
                }

                // wave spawning system
                static sf::Clock waveClock;
                if (waveClock.getElapsedTime().asSeconds() > 8.0f)
                {
                    int waveType = rand() % 3;

                    if (waveType == 0) {
                        for (int idx = 0; idx < 5; idx++) {
                            float waveX = 100.0f + (idx * 150.0f);
                            if (levelmanager.getLevel() == 1) {
                                enemies.add(new Enemy(waveX, -40.0f, levelmanager.getEnemySpeed()));
                            }
                            else if (levelmanager.getLevel() == 2) {
                                enemies.add(new FastEnemy(waveX, -40.0f));
                            }
                            else {
                                enemies.add(new PatternEnemy(waveX, -40.0f));
                            }
                        }
                    }
                    else if (waveType == 1) {
                        for (int idx = 0; idx < 5; idx++) {
                            float waveX = 300.0f + (idx * 80.0f);
                            float waveY = -40.0f - (abs(idx - 2) * 40.0f);
                            if (levelmanager.getLevel() >= 2) {
                                enemies.add(new FastEnemy(waveX, waveY));
                            }
                            else {
                                enemies.add(new Enemy(waveX, waveY, levelmanager.getEnemySpeed()));
                            }
                        }
                    }
                    else {
                        for (int idx = 0; idx < 4; idx++) {
                            float waveX = 50.0f + (idx * 200.0f);
                            float waveY = -40.0f - (idx * 50.0f);
                            if (levelmanager.getLevel() == 3) {
                                enemies.add(new PatternEnemy(waveX, waveY));
                            }
                            else {
                                enemies.add(new Enemy(waveX, waveY, levelmanager.getEnemySpeed()));
                            }
                        }
                    }

                    waveClock.restart();
                }

                if (powerupclock.getElapsedTime().asSeconds() > 5.0f)
                {
                    int type = rand() % 3;
                    float px = static_cast<float>(rand() % 700);
                    if (type == 0)
                        powerups.add(new ShieldPowerUp(px, -20.0f));
                    else if (type == 1)
                        powerups.add(new FireRatePowerUp(px, -20.0f));
                    else
                        powerups.add(new RepairPowerUp(px, -20.0f));
                    powerupclock.restart();
                }

                bullets.updateAll();
                enemyBullets.updateAll();
                enemies.updateAll();
                asteroids.updateAll();
                powerups.updateAll();

                handleCollisions();

                bullets.cleanUp();
                enemyBullets.cleanUp();
                enemies.cleanUp();
                asteroids.cleanUp();
                powerups.cleanUp();

                if (!player->isActive())
                {
                    state = GAME_OVER;

                    cout << endl << "Game Over " << playerName << "!" << endl;
                    cout << "Final Score: " << score << endl;

                    float elapsed = levelclock.getElapsedTime().asSeconds();
                    try {
                        ScoreManager::saveScore(playerName, score, elapsed);
                        ScoreManager::saveBinaryScore(playerName, score, elapsed);
                    }
                    catch (GameException& err) {
                        cout << "Score save error: " << err.getMessage() << endl;
                    }
                }
            }
        }

        void handleCollisions()
        {
            // bullet vs enemy collision
            for (int idx = 0; idx < bullets.getCount(); idx++) {
                Projectile* bullet = bullets.get(idx);
                if (!bullet->isActive())
                    continue;

                BoundingBox bulletBox = bullet->getBounds();

                for (int jdx = 0; jdx < enemies.getCount(); jdx++) {
                    Enemy* enemy = enemies.get(jdx);
                    if (!enemy->isActive())
                        continue;

                    BoundingBox enemyBox = enemy->getBounds();

                    if (bulletBox.intersects(enemyBox)) {
                        bullet->destroy();
                        enemy->takeDamage(20);
                        score += 10;
                        player->addResource(Resource(10));
                    }
                }
            }

            BoundingBox playerBox = player->getBounds();

            // player vs enemy collision
            for (int idx = 0; idx < enemies.getCount(); idx++) {
                Enemy* enemy = enemies.get(idx);
                if (!enemy->isActive())
                    continue;

                BoundingBox enemyBox = enemy->getBounds();

                if (playerBox.intersects(enemyBox)) {
                    enemy->destroy();
                    if (!player->hasShield()) {
                        player->takeDamage(20);
                    }
                }
            }

            // player vs asteroid
            for (int idx = 0; idx < asteroids.getCount(); idx++) {
                Asteroid* asteroid = asteroids.get(idx);
                if (!asteroid->isActive())
                    continue;

                BoundingBox asteroidBox = asteroid->getBounds();

                if (playerBox.intersects(asteroidBox)) {
                    asteroid->destroy();
                    if (!player->hasShield()) {
                        player->takeDamage(10);
                    }
                }
            }

            // player vs powerup
            for (int idx = 0; idx < powerups.getCount(); idx++) {
                PowerUp* powerup = powerups.get(idx);
                if (!powerup->isActive())
                    continue;

                BoundingBox powerupBox = powerup->getBounds();

                if (playerBox.intersects(powerupBox)) {
                    powerup->apply(*player);
                }
            }

            // player vs enemy bullet
            for (int idx = 0; idx < enemyBullets.getCount(); idx++) {
                EnemyBullet* eBullet = enemyBullets.get(idx);
                if (!eBullet->isActive())
                    continue;

                BoundingBox eBulletBox = eBullet->getBounds();

                if (playerBox.intersects(eBulletBox)) {
                    eBullet->destroy();
                    if (!player->hasShield()) {
                        player->takeDamage(15);
                    }
                }
            }
        }

        void render()
        {
            window.clear();

            if (state == SPLASH_SCREEN) {
                window.draw(splashSprite);
            }
            else if (state == MAIN_MENU) {
                window.draw(menuSprite);
            }
            else if (state == NAME_INPUT) {
                window.draw(bgSprite);

                sf::Text titleText;
                titleText.setFont(font);
                titleText.setCharacterSize(50);
                titleText.setFillColor(sf::Color::Yellow);
                titleText.setStyle(sf::Text::Bold);
                titleText.setString("Enter Your Name");
                titleText.setPosition(200, 150);
                window.draw(titleText);

                sf::RectangleShape inputBox(sf::Vector2f(400, 60));
                inputBox.setPosition(200, 300);
                inputBox.setFillColor(sf::Color(50, 50, 50, 200));
                inputBox.setOutlineColor(sf::Color::Yellow);
                inputBox.setOutlineThickness(3);
                window.draw(inputBox);

                sf::Text nameText;
                nameText.setFont(font);
                nameText.setCharacterSize(35);
                nameText.setFillColor(sf::Color::White);
                nameText.setString(playerName + "_");
                nameText.setPosition(220, 310);
                window.draw(nameText);

                sf::Text instructText;
                instructText.setFont(font);
                instructText.setCharacterSize(20);
                instructText.setFillColor(sf::Color::White);
                instructText.setString("Press ENTER to start | ESC to go back");
                instructText.setPosition(180, 450);
                window.draw(instructText);
            }
            else if (state == INSTRUCTIONS) {
                window.draw(instructionsSprite);
            }
            else if (state == LEADERBOARD) {
                window.draw(leaderboardSprite);
                ScoreManager::displayHighScoresOnImage(window, font);
            }
            else if (state == LEVEL_TRANSITION) {
                if (transitionLevel == 1) {
                    window.draw(level1Sprite);
                }
                else if (transitionLevel == 2) {
                    window.draw(level2Sprite);
                }
            }
            else if (state == PLAYING) {
                window.draw(bgSprite);

                player->drawEffects(window);

                player->draw(window);
                bullets.drawAll(window);
                enemyBullets.drawAll(window);
                enemies.drawAll(window);
                asteroids.drawAll(window);
                powerups.drawAll(window);

                string hudString = "Score: " + to_string(score) + "  HP: " + to_string(player->getHealth()) + "  Level: " + to_string(levelmanager.getLevel()) + "  Enemies: " + to_string(Enemy::enemycount);

                if (level3Started) {
                    float timeLeft = level3Duration - level3TimerClock.getElapsedTime().asSeconds();
                    if (timeLeft < 0)
                        timeLeft = 0;
                    hudString += "  Time: " + to_string(static_cast<int>(timeLeft)) + "s";
                }

                hudtext.setString(hudString);
                hudtext.setPosition(10.0f, 10.0f);
                window.draw(hudtext);

                ScoreDisplay::showDetailedStats(*player, window, font);

                // debug key
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    debugPlayerInfo(*player);
                }
            }
            else if (state == PAUSED) {
                window.draw(pauseSprite);
            }
            else if (state == GAME_OVER) {
                window.draw(gameoverSprite);

                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(40);
                scoreText.setFillColor(sf::Color::Yellow);
                scoreText.setStyle(sf::Text::Bold);
                scoreText.setString(to_string(score));
                scoreText.setPosition(550, 385);
                window.draw(scoreText);
            }
            else if (state == GAME_COMPLETE) {
                window.draw(gamecompleteSprite);

                sf::Text congratsText;
                congratsText.setFont(font);
                congratsText.setCharacterSize(30);
                congratsText.setFillColor(sf::Color::Yellow);
                congratsText.setStyle(sf::Text::Bold);
                congratsText.setString("Congratulations " + playerName + "!");
                congratsText.setPosition(250, 300);
                window.draw(congratsText);

                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(40);
                scoreText.setFillColor(sf::Color::Yellow);
                scoreText.setStyle(sf::Text::Bold);
                scoreText.setString(to_string(score));
                scoreText.setPosition(550, 405);
                window.draw(scoreText);
            }

            window.display();
        }

        void resetGame()
        {
            delete player;
            player = new Player(400.0f, 500.0f, &bullets);

            bullets.clear();
            enemyBullets.clear();
            enemies.clear();
            asteroids.clear();
            powerups.clear();

            score = 0;
            bossSpawned = false;
            previousLevel = 1;
            level1Shown = false;
            level2Shown = false;
            level3Started = false;
            frameclock.restart();
            spawnclock.restart();
            powerupclock.restart();
            levelclock.restart();
            levelmanager = LevelManager();
        }
    };

}

#endif
