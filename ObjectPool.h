// Galaxy Wars by Maaz & Danyal

#pragma once
#include "GameException.h"

namespace GalaxyWars {

    
    template <typename T, int MaxSize>

    class ObjectPool 
    {

    private:

        T* pool[MaxSize];
        int count;

    public:


        ObjectPool() 
        {


            count = 0;
            for (int i = 0; i < MaxSize; i++) {
                pool[i] = new T();
            }
        }

        ~ObjectPool()
        {

            for (int i = 0; i < MaxSize; i++) {
                delete pool[i];
            }
        }

        
        ObjectPool(const ObjectPool& other) 
        {


            count = other.count;
            for (int i = 0; i < MaxSize; i++) {

                pool[i] = new T(*other.pool[i]);
            }
        }

        T* get(int index)
        {

            if (index < 0 || index >= MaxSize) throw GameException("Index out of bounds");
            return pool[index];
        }

        T* getAvailable() 
        {


            for (int i = 0; i < MaxSize; i++) {
                if (!pool[i]->isActive()) return pool[i];
            }

            return nullptr;
        }

        int getSize() { return MaxSize; }
    };
}