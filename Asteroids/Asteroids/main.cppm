/*!
 * \brief   Asteroids
 * \author  Karol Pisarski
 * \version 1.9
 * \date    28/09/2022
 */

import <SFML/Graphics.hpp>;

import <list>;
import <typeinfo>;
import <chrono>;
import <thread>;
import <semaphore>;
import <fstream>;


import window;

import animation;
import unit;
import spaceship;
import rocket;
import asteroid;

import explosion;
import label;


import main_menu;
import main_resume;
import main_death;

import global_settings;	// Global game settings


int main() 
{
	std::list<Unit*> units;	// List of units (contains rockets, asteroids, main ship)

	sf::Event main_event;	// Main system event

	sf::RenderWindow app_render_window; // Main game window

	windowSettings( app_render_window );

	sf::Texture texture_background;
	texture_background.setSmooth( true );
	
	sf::Texture texture_spaceship;
	texture_spaceship.setSmooth( true );

	sf::Texture texture_rocket;
	sf::Texture texture_rock;
	sf::Texture texture_explosion;

	sf::Sprite sprite_background;
	sf::Sprite sprite_spaceship;
	
	texture_background.loadFromFile("images/background.jpg");
	texture_spaceship.loadFromFile("images/spaceship.png");
	texture_rocket.loadFromFile("images/fire_blue.png");
	texture_rock.loadFromFile("images/rock.png");
	texture_explosion.loadFromFile("images/explosions/type_C_r.png");
	
	
	sprite_background.setTexture( texture_background );
	//making texture
	sprite_spaceship.setTexture( texture_spaceship );
	//croping texture
	sprite_spaceship.setTextureRect( sf::IntRect( 40, 0, 40, 40 ) );

	Animation animation_rocket( texture_rocket, 0, 0, 32, 64, 16, 0.8 );
	Animation animation_spaceship( texture_spaceship, 40, 0, 40, 40, 1, 0 );
	Animation animation_moving_spaceship( texture_spaceship, 40, 40, 40, 40, 1, 0 );
	Animation animation_rock( texture_rock, 0, 0, 64, 64, 16, 0.2 );
	Animation sExplosion( texture_explosion, 0, 0, 128, 128, 192, 0.7 );


	// napisy na ekranie podczas gry
	Label score( 680, 0, "Score: ", 0 );
	Label life( 640, 870, "Remaining lives: ", 4 );

	app_render_window.clear();


	// generownie menu g³ownego
	main_menu(app_render_window);

	// tworzenie gracza
	Spaceship* player_spaceship = new Spaceship();
	player_spaceship->settings(animation_spaceship, app_render_window.getSize().x / 2, WINDOW_HEIGHT / 2, 270, 20);
	units.push_back(player_spaceship);

	
	// metoda tworzy kilka asteroid na start
	std::thread thread_startSpawnAsteroids(&Asteroid::startSpawnAsteroids, std::ref(units), std::ref(animation_rock));
	thread_startSpawnAsteroids.detach();

	////// g³ówna pêtla programu //////
	while (app_render_window.isOpen()) {
		// kiedy cos sie dzieje to dziala
		while (app_render_window.pollEvent(main_event)) {
			// zamkniecie okna jak jest wymagane
			if (main_event.type == sf::Event::Closed)
				app_render_window.close();

			// strzelanie
			if (main_event.type == sf::Event::KeyPressed) {
				if (main_event.key.code == sf::Keyboard::Space or main_event.key.code == sf::Mouse::Right) {
					Rocket* r = new Rocket();
					r->settings(animation_rocket, player_spaceship->x, player_spaceship->y, player_spaceship->angle, 10);
					units.push_back(r);
				}
				// tutaj bêdzie resume game
				if (main_event.key.code == sf::Keyboard::Escape) {
					main_resume(app_render_window);
				}
			}
			// sprawdzanie flag ruchu
			player_spaceship->updateFlags(sprite_spaceship, main_event, app_render_window);

		}
		// po sprawdzeniu flag aktualizacja XY
		player_spaceship->update(app_render_window);

		// animacja jak statek porusza siê w przód
		if (player_spaceship->front_move)  
			player_spaceship->anim = animation_moving_spaceship;
		else 
			player_spaceship->anim = animation_spaceship;


		for (auto first_object : units)
			for (auto second_object : units) {
				// tutaj trzeba z andem dodaæ jeszcze jeden warunek
				// mozna zakodowac obiekty
				// skorzystaj z typeid
				// type id zwraca inta i jak sa takie same to = 0
				//
 				if (!strcmp(typeid(*first_object).name(), "class Rocket") and !strcmp(typeid(*second_object).name(), "class Asteroid")) {
					if (Unit::isCollide(first_object, second_object)) {
						first_object->life = false;
						second_object->life = false;

						score.iterator++;	

						Explosion* e = new Explosion();
						e->settings(sExplosion, first_object->x, second_object->y);
						units.push_back(e);
					}
				}
				// sprobuj regexem szukac spaceship, asteroidy i rakiety
				if (!strcmp(typeid(*first_object).name(), "class Spaceship") and !strcmp(typeid(*second_object).name(), "class Asteroid"))
					if (Unit::isCollide(first_object, second_object)) {

						life.iterator--;

						if (life.iterator <= 0) {
							second_object->life = false;

							player_spaceship->settings(animation_spaceship, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 270, 20);
							player_spaceship->dx = 0; player_spaceship->dy = 0;
							std::thread thread_velocityToZero(&Spaceship::velocityToZero, player_spaceship);
							thread_velocityToZero.detach();

							main_death(app_render_window, score);

							// zycie z powrotem na 4
							life.iterator = 4;
							score.iterator = 0;

							std::thread thread_deleteAllAstroids(&Asteroid::deleteAllAsteroids, std::ref(units));
							std::thread thread_deleteAllRockets(&Rocket::deleteAllRockets, std::ref(units));
							std::thread thread_delExplosion(&Explosion::delExplosion, std::ref(units));

							std::thread thread_startSpawnAsteroids(&Asteroid::startSpawnAsteroids, std::ref(units), std::ref(animation_rock));
							// semafor aby obiekty nie usuwa³y siê na wzajem
							std::counting_semaphore<1> semaphore(0);

							thread_deleteAllAstroids.detach();
							semaphore.release();

							thread_deleteAllRockets.detach();
							thread_delExplosion.detach();

							semaphore.acquire();
							thread_startSpawnAsteroids.detach();

						}
						else {
							second_object->life = false;
							Explosion* e = new Explosion();
							e->settings(sExplosion, second_object->x, second_object->y);
							units.push_back(e);
						}
					}

			}
		// popraw te w¹tki bo nadal nie dzia³aja
		// 
		// usuwanie animacji eksplozji
		Explosion::delExplosion(units);
		/*std::thread thread_delExplosion(&Explosion::delExplosion, std::ref(units));

		thread_delExplosion.detach();*/

		// tworzenie nowych asteroid
		Asteroid::spawnAsteroids(units, animation_rock);
		/*std::thread thread_spawnAsteroids(&Asteroid::spawnAsteroids, std::ref(units), animation_rock);

		thread_spawnAsteroids.detach();*/

		// aktualizowanie pozycji jednostek oraz usuwanie nie¿ywych objektów
		Unit::updateAndDeleteDeadObjects(units);

		/*std::thread thread_updateAndDeleteDeadObjects(&Unit::updateAndDeleteDeadObjects, std::ref(units));
		thread_updateAndDeleteDeadObjects.detach();*/


		app_render_window.draw(sprite_background);

		// nie rysuje obiektow prawidlowo
		Unit::drawObjects(app_render_window, units);
		/*std::thread thread_drawObjects(&Unit::drawObjects, std::ref(app_render_window), std::ref(units));
		thread_drawObjects.join();*/
	

		score.updateIterator();
		life.updateIterator();

		score.draw(app_render_window);
		life.draw(app_render_window);

		app_render_window.display();
	}

	return 0;
}