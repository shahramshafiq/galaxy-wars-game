#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "BoundingBox.h"

namespace GalaxyWars {

    class GameObject {
    protected:
        sf::Sprite sprite;
        sf::Texture texture;
        float x, y;
        float speed;
        bool active;
        float width, height;
        float scaleX, scaleY;

    public:
        GameObject(float startx, float starty, float spd) : x(startx), y(starty), speed(spd), active(true), width(40), height(40), scaleX(1.0f), scaleY(1.0f) {}

        virtual ~GameObject() {}

        virtual void update() = 0;

        virtual void draw(sf::RenderWindow& window)
        {
            if (active) {
                sprite.setPosition(x, y);
                window.draw(sprite);
            }
        }

        bool isActive() const {
            return active;
        }

        void destroy() {
            active = false;
        }

        BoundingBox getBounds() const {
            float actualWidth = width * scaleX;
            float actualHeight = height * scaleY;
            return BoundingBox(x, y, actualWidth, actualHeight);
        }

        float getX() const {
            return x;
        }

        float getY() const {
            return y;
        }

        void updateCollisionFromTexture()
        {
            if (texture.getSize().x > 0 && texture.getSize().y > 0)
            {
                width = static_cast<float>(texture.getSize().x);
                height = static_cast<float>(texture.getSize().y);
            }
        }

        void setScale(float sx, float sy) {
            scaleX = sx;
            scaleY = sy;
            sprite.setScale(sx, sy);
        }
    };

}

#endif
