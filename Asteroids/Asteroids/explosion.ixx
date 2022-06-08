export module explosion;


import unit;

import <typeinfo>;

export class Explosion : public Unit {
public:

    static void delExplosion(std::list<Unit*>& units) {
        for (auto el : units)
            if (!strcmp(typeid(*el).name(), "class Explosion"))
                if (el->anim.isEnd())
                    el->life = false;

    }
};