#pragma once
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

namespace GalaxyWars {

    class BoundingBox {
    public:
        float x, y, width, height;

        BoundingBox() : x(0), y(0), width(0), height(0) {}

        BoundingBox(float px, float py, float w, float h) : x(px), y(py), width(w), height(h) {}

        BoundingBox(const BoundingBox& other) : x(other.x), y(other.y), width(other.width), height(other.height) {}

        bool intersects(const BoundingBox& other) const
        {
            // check if boxes overlap
            bool result = (x < other.x + other.width && x + width > other.x && y < other.y + other.height && y + height > other.y);
            return result;
        }
    };

}

#endif
