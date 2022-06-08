export module spaceship;

import unit;


// jak bedziesz czytal z pliku to walnij metode do tego
export class Spaceship : public Unit {
public:

bool front_move = false, left_move = false, right_move = false;

// Wykrywanie oraz ustawianie flag zwiazanych z movementem
void updateFlags(sf::Sprite& s, sf::Event& event, sf::RenderWindow& app) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::W:
		case sf::Keyboard::Up:
			front_move = true;
			break;
		case sf::Keyboard::A:
		case sf::Keyboard::Left:
			left_move = true;
			break;
		case sf::Keyboard::D:
		case sf::Keyboard::Right:
			right_move = true;
			break;
		default: break;
		}
	}
	
	if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				front_move = false;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				left_move = false;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				right_move = false;
				break;
			default: break;
		}
	}
}
// Movement ze skrecaniem uzalezniona od zmiany czasu i fps
void update(sf::RenderWindow& app) {
	// skrecanie ogarnij to dobrze
		if (right_move)
			angle += 3.0f;
		if (left_move)
			angle -= 3.0f;
		// pryndosc
		if (front_move) {
			dy += sin(angle * DegToRad) * 4.0f;
			dx += cos(angle * DegToRad) * 4.0f;
		}
		// wytracanie predkosci git tu nie trzeba dt
		else {
			dx *= 0.8;
			dy *= 0.8;
		}

		// przyspieszenie
		int maxSpeed = 2.25f;
		// dlugosc wektora
		float speed = sqrt(dx * dx + dy * dy);
		if (speed > maxSpeed) {
			dx *= maxSpeed / speed;
			dy *= maxSpeed / speed;
		}
		x += dx;
		y += dy;

		// sprawdzenie granic mapy zrobione super
		if (x < 20)
			x = 20;
		if (x > (int)app.getSize().x - 20)
			x = app.getSize().x - 20;
		if (y < 20)
			y = 20;
		if (y > (int)app.getSize().y - 20)
			y = app.getSize().y - 20;
	}



void velocityToZero() {
	dx = 0;
	dy = 0;
	front_move = false;
	left_move = false;
	right_move = false;
	}
};