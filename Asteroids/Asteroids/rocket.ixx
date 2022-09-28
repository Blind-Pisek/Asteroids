export module rocket;

import <cmath>;

import unit;
import global_settings;


export class Rocket : public Unit {
public:

    void  update() {
        // speed na 8 jest git       to 8 to predkosc rakiety
        dx = cos(angle * DegToRad) * 4;
        dy = sin(angle * DegToRad) * 4;
     
        // bardziej realistyczne
        //angle+=rand()% 7 - 3;  
        x += dx;
        y += dy;

        if (x > WINDOW_WIDTH or x < 0 or y > WINDOW_HEIGHT or y < 0)
            life = false;
    }

    static void deleteAllRockets(std::list<Unit*>& units) {
        for (auto object : units) {
            if (!strcmp(typeid(*object).name(), "class Rocket"))
                object->life = false;
        }
    }

};

