export module unit;

export import <list>;

import animation;
export import global_settings;


export class Unit {
protected:

	const float DegToRad = 0.017453f;

public:

	float x = 0, y = 0, dx = 0, dy = 0, angle = 0, radius = 0;
	bool life;
	Animation anim;

	Unit() {
		life = true;
	}

	virtual void update() {};

	void settings(Animation& a, int X, int Y, float Angle = 0, int R = 1) {
		anim = a;
		x = X; y = Y;
		angle = Angle;
		radius = R;
	}

	static bool isCollide(Unit* a, Unit* b) {
		return (b->x - a->x) * (b->x - a->x) +
			(b->y - a->y) * (b->y - a->y) <
			(a->radius + b->radius) * (a->radius + b->radius);
	}


	void draw(sf::RenderWindow& app) {
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);
	}

	static void updateAndDeleteDeadObjects(std::list<Unit*>& units) {
		for (auto i = units.begin(); i != units.end();) {
			Unit* u = *i;
			u->update();
			u->anim.update();
			if (u->life == false) {
				i = units.erase(i);
				delete u;
			}
			else i++;
		}
	}

	static void drawObjects(sf::RenderWindow& rw, std::list<Unit*> &units) {
		for (auto el : units)
			el->draw(rw);
	}

};