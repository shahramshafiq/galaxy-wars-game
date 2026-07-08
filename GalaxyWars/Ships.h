#pragma once
#ifndef SHIPS_H
#define SHIPS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "GameObject.h"
#include "Resource.h"
#include "GameObjectArray.h"

using namespace std;

namespace GalaxyWars {

    class Projectile;
    class EnemyBullet;
    class Player;

    // particle effect class
    class Particle {
    public:
        sf::CircleShape shape;
        float lifetime;
        float maxLifetime;
        float velocityY;
        bool alive;

        Particle() : lifetime(0), maxLifetime(0), velocityY(0), alive(false) {}

        Particle(float x, float y, float radius, sf::Color color, float life) : lifetime(life), maxLifetime(life), velocityY(-50.0f), alive(true) {
            shape.setRadius(radius);
            shape.setPosition(x, y);
            shape.setFillColor(color);
        }

        Particle(const Particle& other) : lifetime(other.lifetime), maxLifetime(other.maxLifetime), velocityY(other.velocityY), alive(other.alive) {
            shape = other.shape;
        }

        void update(float deltaTime) {
            if (!alive)
                return;

            lifetime -= deltaTime;
            shape.move(0, velocityY * deltaTime);

            if (lifetime <= 0) {
                alive = false;
                return;
            }

            sf::Color col = shape.getFillColor();
            col.a = static_cast<sf::Uint8>(255 * (lifetime / maxLifetime));
            shape.setFillColor(col);
        }

        bool isAlive() const {
            return alive;
        }

        void draw(sf::RenderWindow& window) {
            if (alive) {
                window.draw(shape);
            }
        }
    };

    class Projectile : public GameObject {
        float velX;
        float velY;

    public:
        Projectile(float startx, float starty, float inheritedVelocityX) : GameObject(startx, starty, 7.0f), velX(inheritedVelocityX), velY(-7.0f) {

            if (!texture.loadFromFile("Sprites/bullet.png")) {
                cout << "Bullet texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
        }

        Projectile(const Projectile& other) : GameObject(other.x, other.y, other.speed), velX(other.velX), velY(other.velY) {
            texture = other.texture;
            sprite.setTexture(texture);
            width = other.width;
            height = other.height;
            scaleX = other.scaleX;
            scaleY = other.scaleY;
            active = other.active;
        }

        void update() override
        {
            x += velX;
            y += velY;

            if (y < -20)
                active = false;
            if (x < -20 || x > 820)
                active = false;
        }
    };

    class EnemyBullet : public GameObject {
    public:
        EnemyBullet(float startx, float starty) : GameObject(startx, starty, 4.0f) {

            if (!texture.loadFromFile("Sprites/enemy_bullet.png")) {
                cout << "Enemy bullet texture not found" << endl;
            }
            sprite.setTexture(texture);

            // remove white background
            sf::Image img = texture.copyToImage();
            img.createMaskFromColor(sf::Color::White);
            texture.loadFromImage(img);
            sprite.setTexture(texture);

            updateCollisionFromTexture();
            setScale(1.0f, 1.0f);
        }

        EnemyBullet(const EnemyBullet& other) : GameObject(other.x, other.y, other.speed) {
            texture = other.texture;
            sprite.setTexture(texture);
            width = other.width;
            height = other.height;
            scaleX = other.scaleX;
            scaleY = other.scaleY;
            active = other.active;
        }

        void update() override {
            y += speed;
            if (y > 620)
                active = false;
        }
    };

    class Ship : public GameObject {
    protected:
        int health;
    public:
        Ship(float startx, float starty, float spd, int hp) : GameObject(startx, starty, spd), health(hp) {}

        void takeDamage(int dmg)
        {
            health -= dmg;
            if (health <= 0)
                destroy();
        }

        int getHealth() const {
            return health;
        }

        void setHealth(int h) {
            health = h;
        }
    };

    class Player : public Ship {
        GameObjectArray<Projectile>* bullets;
        Resource collectedresources;
        bool shielded;
        float firecooldown;
        float firetimer;
        float shieldDuration;
        float fireRateDuration;
        bool autoFire;

        float lastX, lastY;
        float velX, velY;

        float shieldPulseTimer;
        float fireEffectRotation;
        static const int MAX_PARTICLES = 50;
        Particle healingParticles[MAX_PARTICLES];
        int particleCount;

        friend class ScoreDisplay;
        friend void debugPlayerInfo(const Player& p);

    public:
        Player(float startx, float starty, GameObjectArray<Projectile>* bulletlist) : Ship(startx, starty, 5.0f, 100), bullets(bulletlist), collectedresources(0), shielded(false), firecooldown(0.15f), firetimer(0.0f), shieldDuration(0.0f), fireRateDuration(0.0f), autoFire(false), lastX(startx), lastY(starty), velX(0.0f), velY(0.0f), shieldPulseTimer(0.0f), fireEffectRotation(0.0f), particleCount(0) {

            if (!texture.loadFromFile("Sprites/player.png")) {
                cout << "Player texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.5f, 0.5f);
        }

        Player(const Player& other) : Ship(other.x, other.y, other.speed, other.health), bullets(other.bullets), collectedresources(other.collectedresources), shielded(other.shielded), firecooldown(other.firecooldown), firetimer(other.firetimer), shieldDuration(other.shieldDuration), fireRateDuration(other.fireRateDuration), autoFire(other.autoFire), lastX(other.lastX), lastY(other.lastY), velX(other.velX), velY(other.velY), shieldPulseTimer(other.shieldPulseTimer), fireEffectRotation(other.fireEffectRotation), particleCount(other.particleCount) {

            texture = other.texture;
            sprite.setTexture(texture);
            width = other.width;
            height = other.height;
            scaleX = other.scaleX;
            scaleY = other.scaleY;
            active = other.active;

            for (int idx = 0; idx < MAX_PARTICLES; idx++) {
                healingParticles[idx] = other.healingParticles[idx];
            }

            cout << "Player copied with health: " << health << endl;
        }

        void update() override
        {
            lastX = x;
            lastY = y;

            float spriteWidth = width * scaleX;
            float spriteHeight = height * scaleY;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && x > 0)
                x -= speed;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && x + spriteWidth < 800)
                x += speed;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && y > 0)
                y -= speed;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && y + spriteHeight < 600)
                y += speed;

            velX = x - lastX;
            velY = y - lastY;
        }

        void tick(float deltaTime)
        {
            firetimer += deltaTime;

            if (autoFire && firetimer >= firecooldown) {
                shootAutomatic();
                firetimer = 0.0f;
            }

            if (shielded)
            {
                shieldDuration -= deltaTime;
                shieldPulseTimer += deltaTime;
                if (shieldDuration <= 0) {
                    shielded = false;
                }
            }

            if (firecooldown < 0.25f)
            {
                fireRateDuration -= deltaTime;
                fireEffectRotation += deltaTime * 200.0f;
                if (fireRateDuration <= 0) {
                    firecooldown = 0.25f;
                }
            }

            for (int idx = 0; idx < MAX_PARTICLES; idx++) {
                if (healingParticles[idx].isAlive()) {
                    healingParticles[idx].update(deltaTime);
                }
            }
        }

        void shootAutomatic() {
            float spriteWidth = width * scaleX;
            float bulletX = x + (spriteWidth / 2.0f) - 55.0f;
            bullets->add(new Projectile(bulletX, y, velX));
        }

        void shoot() {
            if (firetimer >= firecooldown) {
                shootAutomatic();
                firetimer = 0.0f;
            }
        }

        void addResource(Resource r) {
            collectedresources = collectedresources + r;
        }

        int getScore() const {
            return collectedresources.getValue();
        }

        void heal(int amount)
        {
            health += amount;
            if (health > 100)
                health = 100;

            float centerX = x + (width * scaleX) / 2.0f;
            float centerY = y + (height * scaleY) / 2.0f;

            int created = 0;
            for (int idx = 0; idx < MAX_PARTICLES && created < 15; idx++)
            {
                if (!healingParticles[idx].isAlive()) {
                    float offsetX = (rand() % 60) - 30.0f;
                    float offsetY = (rand() % 60) - 30.0f;
                    healingParticles[idx] = Particle(centerX + offsetX, centerY + offsetY, 3.0f, sf::Color::Green, 2.0f);
                    created++;
                }
            }
        }

        void setShield(bool s) {
            shielded = s;
            if (s) {
                shieldDuration = 5.0f;
                shieldPulseTimer = 0.0f;
            }
        }

        bool hasShield() const {
            return shielded;
        }

        void setFireCooldown(float c) {
            firecooldown = c;
            if (c < 0.25f) {
                fireRateDuration = 5.0f;
                fireEffectRotation = 0.0f;
            }
        }

        void toggleAutoFire() {
            autoFire = !autoFire;
        }

        bool isAutoFiring() const {
            return autoFire;
        }

        float getVelocityX() const {
            return velX;
        }

        void drawEffects(sf::RenderWindow& window)
        {
            float centerX = x + (width * scaleX) / 2.0f;
            float centerY = y + (height * scaleY) / 2.0f;

            if (shielded)
            {
                float pulseSize = 60.0f + sin(shieldPulseTimer * 5.0f) * 10.0f;
                sf::CircleShape shieldBubble(pulseSize);
                shieldBubble.setOrigin(pulseSize, pulseSize);
                shieldBubble.setPosition(centerX, centerY);
                shieldBubble.setFillColor(sf::Color(0, 255, 255, 50));
                shieldBubble.setOutlineThickness(3);
                shieldBubble.setOutlineColor(sf::Color(0, 200, 255, 150));
                window.draw(shieldBubble);

                sf::CircleShape innerGlow(pulseSize - 10.0f);
                innerGlow.setOrigin(pulseSize - 10.0f, pulseSize - 10.0f);
                innerGlow.setPosition(centerX, centerY);
                innerGlow.setFillColor(sf::Color(100, 200, 255, 30));
                window.draw(innerGlow);
            }

            if (firecooldown < 0.25f)
            {
                for (int idx = 0; idx < 8; idx++) {
                    float angle = (fireEffectRotation + idx * 45.0f) * 3.14159f / 180.0f;
                    float radius = 40.0f;
                    float px = centerX + cos(angle) * radius;
                    float py = centerY + sin(angle) * radius;

                    sf::CircleShape fireParticle(5.0f);
                    fireParticle.setPosition(px - 5.0f, py - 5.0f);

                    if (idx % 2 == 0) {
                        fireParticle.setFillColor(sf::Color(255, 100, 0, 200));
                    }
                    else {
                        fireParticle.setFillColor(sf::Color(255, 50, 0, 200));
                    }

                    window.draw(fireParticle);
                }

                sf::CircleShape fireGlow(35.0f);
                fireGlow.setOrigin(35.0f, 35.0f);
                fireGlow.setPosition(centerX, centerY);
                fireGlow.setFillColor(sf::Color(255, 150, 0, 30));
                window.draw(fireGlow);
            }

            for (int idx = 0; idx < MAX_PARTICLES; idx++) {
                if (healingParticles[idx].isAlive()) {
                    healingParticles[idx].draw(window);
                }
            }
        }
    };

    class Enemy : public Ship {
    public:
        static int enemycount;

        Enemy(float startx, float starty, float spd) : Ship(startx, starty, spd, 20) {
            if (!texture.loadFromFile("Sprites/enemy_plane.png")) {
                cout << "Enemy texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.3f, 0.3f);
            enemycount++;
        }

        Enemy(const Enemy& other) : Ship(other.x, other.y, other.speed, other.health) {
            texture = other.texture;
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.3f, 0.3f);
            enemycount++;
            cout << "Enemy copied at position: (" << x << ", " << y << ")" << endl;
        }

        virtual ~Enemy() {
            enemycount--;
        }

        virtual void update() override {
            y += speed;
            if (y > 620)
                active = false;
        }
    };

    class FastEnemy : public Enemy {
    public:
        FastEnemy(float startx, float starty) : Enemy(startx, starty, 4.0f) {
            if (!texture.loadFromFile("Sprites/dangerousenemy.png")) {
                cout << "Fast enemy texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.3f, 0.3f);
        }

        FastEnemy(const FastEnemy& other) : Enemy(other) {
            cout << "FastEnemy copied" << endl;
        }
    };

    class PatternEnemy : public Enemy {
        float angle;
        float shootTimer;
        float shootCooldown;
        GameObjectArray<EnemyBullet>* enemyBullets;

    public:
        PatternEnemy(float startx, float starty, GameObjectArray<EnemyBullet>* bullets = nullptr) : Enemy(startx, starty, 2.5f), angle(0.0f), shootTimer(0.0f), shootCooldown(1.0f), enemyBullets(bullets) {
            if (!texture.loadFromFile("Sprites/enemy_spaceship.png")) {
                cout << "Pattern enemy texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.3f, 0.3f);
        }

        PatternEnemy(const PatternEnemy& other) : Enemy(other), angle(other.angle), shootTimer(other.shootTimer), shootCooldown(other.shootCooldown), enemyBullets(other.enemyBullets) {
            cout << "PatternEnemy copied" << endl;
        }

        void update() override
        {
            y += speed;
            angle += 0.1f;
            x += sin(angle) * 3.0f;

            float spriteWidth = width * scaleX;
            if (x < 0)
                x = 0;
            if (x + spriteWidth > 800)
                x = 800 - spriteWidth;

            shootTimer += 0.016f;
            if (shootTimer >= shootCooldown && enemyBullets != nullptr && y > -50 && y < 600)
            {
                float bulletX = x + (width * scaleX / 2.0f) - 5.0f;
                enemyBullets->add(new EnemyBullet(bulletX, y + height * scaleY));
                shootTimer = 0.0f;
            }

            if (y > 620)
                active = false;
        }

        void setEnemyBullets(GameObjectArray<EnemyBullet>* bullets) {
            enemyBullets = bullets;
        }
    };

    class BossEnemy : public Enemy {
        float shootTimer;
        int phase;
    public:
        BossEnemy(float startx, float starty) : Enemy(startx, starty, 1.5f), shootTimer(0), phase(1) {
            health = 100;
            if (!texture.loadFromFile("Sprites/enemy_plane.png")) {
                cout << "Boss texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.6f, 0.6f);
        }

        BossEnemy(const BossEnemy& other) : Enemy(other), shootTimer(other.shootTimer), phase(other.phase) {
            cout << "BossEnemy copied" << endl;
        }

        void update() override
        {
            y += speed * 0.5f;
            x += sin(y * 0.05f) * 2.0f;

            float spriteWidth = width * scaleX;
            if (x < 0)
                x = 0;
            if (x + spriteWidth > 800)
                x = 800 - spriteWidth;

            if (y > 620)
                active = false;
        }
    };

    void debugPlayerInfo(const Player& p) {
        cout << "DEBUG - Health: " << p.health << " Resources: " << p.collectedresources.getValue() << " Shield: " << (p.shielded ? "ON" : "OFF") << " AutoFire: " << (p.autoFire ? "ON" : "OFF") << " VelX: " << p.velX << endl;
    }

    class ScoreDisplay {
    public:
        static void showDetailedStats(const Player& p, sf::RenderWindow& window, sf::Font& font)
        {
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(16);
            text.setFillColor(sf::Color::Yellow);

            string stats = "HP: " + to_string(p.health) + " | Score: " + to_string(p.collectedresources.getValue()) + " | Shield: " + (p.shielded ? "ACTIVE" : "OFF") + " | AutoFire: " + (p.autoFire ? "ON" : "OFF") + " [Press F]";

            text.setString(stats);
            text.setPosition(10, 40);
            window.draw(text);
        }
    };

}

#endif
