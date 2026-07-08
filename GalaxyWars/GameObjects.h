#pragma once
#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameObject.h"
#include "Ships.h"

using namespace std;

namespace GalaxyWars {

    class Asteroid : public GameObject {
    public:
        Asteroid(float startx, float starty, float spd = 3.0f) : GameObject(startx, starty, spd) {
            if (!texture.loadFromFile("Sprites/asteroid.png")) {
                cout << "Asteroid texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.2f, 0.2f);
        }

        Asteroid(const Asteroid& other) : GameObject(other.x, other.y, other.speed) {
            texture = other.texture;
            sprite.setTexture(texture);
            width = other.width;
            height = other.height;
            scaleX = other.scaleX;
            scaleY = other.scaleY;
            active = other.active;
            cout << "Asteroid copied" << endl;
        }

        void update() override {
            y += speed;
            if (y > 620)
                active = false;
        }
    };

    // powerup base class
    class PowerUp : public GameObject {
    public:
        PowerUp(float startx, float starty) : GameObject(startx, starty, 2.5f) {}
        virtual void apply(Player& player) = 0;
    };

    class ShieldPowerUp : public PowerUp {
    public:
        ShieldPowerUp(float startx, float starty) : PowerUp(startx, starty) {
            if (!texture.loadFromFile("Sprites/shield_powerup.png")) {
                cout << "Shield powerup texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.2f, 0.2f);
        }

        ShieldPowerUp(const ShieldPowerUp& other) : PowerUp(other.x, other.y) {
            texture = other.texture;
            sprite.setTexture(texture);
            width = other.width;
            height = other.height;
            scaleX = other.scaleX;
            scaleY = other.scaleY;
            active = other.active;
            speed = other.speed;
        }

        void update() override {
            y += speed;
            if (y > 620)
                active = false;
        }

        void apply(Player& player) override {
            player.setShield(true);
            active = false;
        }
    };

    class FireRatePowerUp : public PowerUp {
    public:
        FireRatePowerUp(float startx, float starty) : PowerUp(startx, starty) {
            if (!texture.loadFromFile("Sprites/fire_powerup.png")) {
                cout << "Fire powerup texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.2f, 0.2f);
        }

        FireRatePowerUp(const FireRatePowerUp& other) : PowerUp(other.x, other.y) {
            texture = other.texture;
            sprite.setTexture(texture);
            width = other.width;
            height = other.height;
            scaleX = other.scaleX;
            scaleY = other.scaleY;
            active = other.active;
            speed = other.speed;
        }

        void update() override {
            y += speed;
            if (y > 620)
                active = false;
        }

        void apply(Player& player) override {
            player.setFireCooldown(0.08f);
            active = false;
        }
    };

    class RepairPowerUp : public PowerUp {
    public:
        RepairPowerUp(float startx, float starty) : PowerUp(startx, starty) {
            if (!texture.loadFromFile("Sprites/coin.png")) {
                cout << "Repair powerup texture not found" << endl;
            }
            sprite.setTexture(texture);
            updateCollisionFromTexture();
            setScale(0.8f, 0.8f);
        }

        RepairPowerUp(const RepairPowerUp& other) : PowerUp(other.x, other.y) {
            texture = other.texture;
            sprite.setTexture(texture);
            width = other.width;
            height = other.height;
            scaleX = other.scaleX;
            scaleY = other.scaleY;
            active = other.active;
            speed = other.speed;
        }

        void update() override {
            y += speed;
            if (y > 620)
                active = false;
        }

        void apply(Player& player) override {
            player.heal(30);
            active = false;
        }
    };

}

#endif
