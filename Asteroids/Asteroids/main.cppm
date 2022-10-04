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

	// Could have made RenderWindow global object
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
	
	
	sprite_background.setTexture( texture_background );	// Making texture
	sprite_spaceship.setTexture( texture_spaceship );
	
	sprite_spaceship.setTextureRect( sf::IntRect( 40, 0, 40, 40 ) );	// Croping texture

	Animation animation_rocket( texture_rocket, 0, 0, 32, 64, 16, 0.8 );
	Animation animation_spaceship( texture_spaceship, 40, 0, 40, 40, 1, 0 );
	Animation animation_moving_spaceship( texture_spaceship, 40, 40, 40, 40, 1, 0 );
	Animation animation_rock( texture_rock, 0, 0, 64, 64, 16, 0.2 );
	Animation animation_explosion( texture_explosion, 0, 0, 128, 128, 192, 0.7 );


	// Labels shown during gameplay
	Label score( 680, 0, "Score: ", 0 );
	Label life( 640, 870, "Remaining lives: ", NUMBER_OF_LIVES );

	app_render_window.clear();	// Clear whole window

	main_menu(app_render_window);	// Main loop before game starst

	// Creating main spaceship and placing it on the centre of the screen
	Spaceship* player_spaceship = new Spaceship();
	player_spaceship->settings( animation_spaceship, app_render_window.getSize().x / 2,
								app_render_window.getSize().y / 2, 270, 20 );	// Could have used global variables to pass window size
	units.push_back(player_spaceship);

	// Creating couple of asteroids on start
	std::thread thread_startSpawnAsteroids( &Asteroid::startSpawnAsteroids, std::ref( units ), 
											std::ref( animation_rock ) );
	thread_startSpawnAsteroids.detach();

	////// MAIN GAME LOOP //////
	while ( app_render_window.isOpen() ) 
	{
		// If there is some kind of event
		while ( app_render_window.pollEvent( main_event ) ) 
		{
			// Closes window if it is required
			if ( main_event.type == sf::Event::Closed )
				app_render_window.close();

			if ( main_event.type == sf::Event::KeyPressed ) // Shooting
			{
				if ( main_event.key.code == sf::Keyboard::Space or 
					 main_event.key.code == sf::Mouse::Right )
				{
					Rocket* new_rocket = new Rocket();
					new_rocket->settings( animation_rocket, player_spaceship->x,
										  player_spaceship->y, player_spaceship->angle, 10 );
					units.push_back( new_rocket );
				}
				// If ESC key is pressed the game pauses
				if ( main_event.key.code == sf::Keyboard::Escape ) 
				{
					main_resume( app_render_window ); // Main loop of resuming game 
				}
			}	/* if key pressed */

			// Check movement flags
			player_spaceship->updateFlags( sprite_spaceship, main_event, 
										   app_render_window );

		}	/* while */

		// After activating movement key time to update spaceship coordinates
		player_spaceship->update( app_render_window );

		// Animation of frontal spaceship movement
		if ( player_spaceship->front_move )  
			player_spaceship->anim = animation_moving_spaceship;
		else 
			player_spaceship->anim = animation_spaceship;


		for ( auto first_object : units )	// Checking colision with pair of objects
			for ( auto second_object : units )	
			{
				// If rocket and asteroid collide
 				if ( !strcmp( typeid(*first_object).name(), "class Rocket" ) and 
					 !strcmp( typeid(*second_object).name(), "class Asteroid" ) and
					 Unit::isCollide( first_object, second_object ) )
				{	 
					first_object->life = false;
					second_object->life = false;

					score.iterator++;

					Explosion* e = new Explosion();
					e->settings( animation_explosion, first_object->x, second_object->y );
				
				}

				// If spaceship is hit by asteroid
				if ( !strcmp( typeid(*first_object).name(), "class Spaceship" ) and 
					 !strcmp( typeid(*second_object).name(), "class Asteroid")  and
					 Unit::isCollide( first_object, second_object ) )
				{

					life.iterator--;	// Decrement number of lives

					if (life.iterator <= 0) 
					{
						second_object->life = false;

						player_spaceship->settings( animation_spaceship, WINDOW_WIDTH / 2, 
													WINDOW_HEIGHT / 2, 270, 20 );

						player_spaceship->dx = 0; player_spaceship->dy = 0;

						std::thread thread_velocityToZero(&Spaceship::velocityToZero, player_spaceship);
						thread_velocityToZero.detach();

						main_death( app_render_window, score );	// Death screen main loop

						life.iterator = NUMBER_OF_LIVES;	// If game restarted life iterator 
						score.iterator = 0;

						std::thread thread_deleteAllAstroids(&Asteroid::deleteAllAsteroids, std::ref(units));
						std::thread thread_deleteAllRockets(&Rocket::deleteAllRockets, std::ref(units));
						std::thread thread_delExplosion(&Explosion::delExplosion, std::ref(units));

						std::thread thread_startSpawnAsteroids(&Asteroid::startSpawnAsteroids, std::ref(units), std::ref(animation_rock));
						// semafor aby obiekty nie usuwa�y si� na wzajem
						std::counting_semaphore<1> semaphore(0);

						thread_deleteAllAstroids.detach();
						semaphore.release();

						thread_deleteAllRockets.detach();
						thread_delExplosion.detach();

						semaphore.acquire();
						thread_startSpawnAsteroids.detach();

					}
					else 
					{
						second_object->life = false;
						Explosion* e = new Explosion();
						e->settings(animation_explosion, second_object->x, second_object->y);
						units.push_back(e);
					}
				}

			} /* for() first & second object */
		// popraw te w�tki bo nadal nie dzia�aja
		// 
		// usuwanie animacji eksplozji
		Explosion::delExplosion(units);
		/*std::thread thread_delExplosion(&Explosion::delExplosion, std::ref(units));

		thread_delExplosion.detach();*/

		// tworzenie nowych asteroid
		Asteroid::spawnAsteroids(units, animation_rock);
		/*std::thread thread_spawnAsteroids(&Asteroid::spawnAsteroids, std::ref(units), animation_rock);

		thread_spawnAsteroids.detach();*/

		// aktualizowanie pozycji jednostek oraz usuwanie nie�ywych objekt�w
		Unit::updateAndDeleteDeadObjects(units);

		/*std::thread thread_updateAndDeleteDeadObjects(&Unit::updateAndDeleteDeadObjects, std::ref(units));
		thread_updateAndDeleteDeadObjects.detach();*/


		app_render_window.draw(sprite_background);

		// nie rysuje obiektow prawidlowo
		Unit::drawObjects(app_render_window, units);
		/*std::thread thread_drawObjects(&Unit::drawObjects, std::ref(app_render_window), std::ref(units));
		thread_drawObjects.join();*/
	
		// Updating labels iterators
		score.updateIterator();
		life.updateIterator();

		// Drawing labels
		score.draw(app_render_window);
		life.draw(app_render_window);

		app_render_window.display();
	}	/* while MAIN GAME LOOP */ 

	return 0;
}