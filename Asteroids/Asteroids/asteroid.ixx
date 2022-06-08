export module asteroid;

import <typeinfo>;

import unit;

import window_size;

export class Asteroid : public Unit {
public:

    Asteroid() {
        dx = rand() % 7 - 3;
        dy = rand() % 7 - 3;
        while (dx == dy) 
            dy = rand() % 4 - 2;

    }

    void update() {
        x += dx;
        y += dy;

        if (x > width) 
            x = 0;  
        if (x < 0) 
            x = width;
        if (y > height) 
            y = 0;  
        if (y < 0) 
            y = height;
    }

    static void startSpawnAsteroids(std::list<Unit*>& units, Animation& animation_of_rock) {
        for (int i = 0; i < 4; i++) {
            Asteroid* asteroids = new Asteroid();
            asteroids->settings(animation_of_rock, rand() % width, rand() % height, rand() % 360, 25);
            units.push_back(asteroids);
        }
    }

    static void spawnAsteroids(std::list<Unit*>& units, Animation animation_of_rock) {
        if (rand() % 150 == 0) {
            Asteroid* asteroids = new Asteroid();
            asteroids->settings(animation_of_rock, 0, rand() % height, rand() % 330 + 10, 25);
            units.push_back(asteroids);
        }
    }

    static void deleteAllAsteroids(std::list<Unit*>& units) {
        for (auto object : units) {
            if (!strcmp(typeid(*object).name(), "class Asteroid"))
                object->life = false;
        } 
    }

};