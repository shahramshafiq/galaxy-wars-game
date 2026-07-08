#pragma once
#ifndef GAMEOBJECTARRAY_H
#define GAMEOBJECTARRAY_H

namespace GalaxyWars {

    template <typename T>
    class GameObjectArray {
    protected:
        T** items;
        int capacity;
        int count;

        void resize()
        {
            int newCapacity = capacity * 2;
            T** newItems = new T * [newCapacity];
            for (int idx = 0; idx < count; idx++) {
                newItems[idx] = items[idx];
            }
            delete[] items;
            items = newItems;
            capacity = newCapacity;
        }

    public:
        GameObjectArray() : capacity(50), count(0) {
            items = new T * [capacity];
        }

        ~GameObjectArray() {
            clear();
            delete[] items;
        }

        void add(T* item)
        {
            if (count >= capacity) {
                resize();
            }
            items[count] = item;
            count++;
        }

        void updateAll() {
            for (int idx = 0; idx < count; idx++)
            {
                if (items[idx]->isActive()) {
                    items[idx]->update();
                }
            }
        }

        void drawAll(sf::RenderWindow& window) {
            for (int idx = 0; idx < count; idx++) {
                if (items[idx]->isActive())
                {
                    items[idx]->draw(window);
                }
            }
        }

        void cleanUp()
        {
            int writeIdx = 0;
            for (int readIdx = 0; readIdx < count; readIdx++)
            {
                if (items[readIdx]->isActive()) {
                    items[writeIdx] = items[readIdx];
                    writeIdx++;
                }
                else {
                    delete items[readIdx];
                }
            }
            count = writeIdx;
        }

        void clear() {
            for (int idx = 0; idx < count; idx++) {
                delete items[idx];
            }
            count = 0;
        }

        T* get(int index) {
            if (index >= 0 && index < count) {
                return items[index];
            }
            return nullptr;
        }

        int getCount() const {
            return count;
        }
    };

}

#endif
