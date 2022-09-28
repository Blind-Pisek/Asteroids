export module main_death;

import <SFML/Graphics.hpp>;

import <thread>;

import death;
import label;

import global_settings;


// modu³ zajmuj¹cy siê wszystkim po œmierci¹

export void main_death(sf::RenderWindow& app_render_window, Label score) {

	sf::Texture menu_texture;
	menu_texture.loadFromFile("images/menu/menu_background2_r.jpg");
	menu_texture.setSmooth(true);

	sf::Sprite sprite;
	sprite.setTexture(menu_texture);

	Death death(WINDOW_WIDTH, WINDOW_HEIGHT);
	sf::Vector2i mouse_coordinates;

	Label max_score(680, 0, "Max score: ", 0);

	max_score.getItFromFile("max_score.txt");

	if (score.iterator > max_score.iterator) {
		max_score.iterator = score.iterator;
		max_score.saveItToFile("max_score.txt");
	}
	max_score.updateIterator();
			

	sf::Event resume_event;

	while (death.main_loop) {
		while (app_render_window.pollEvent(resume_event)) {

			if (resume_event.type == sf::Event::Closed)
				app_render_window.close();

			if (resume_event.type == sf::Event::KeyPressed) {
				if (resume_event.key.code == sf::Keyboard::Escape) {
					app_render_window.close();
					death.main_loop = false;
				}
			}
			mouse_coordinates = sf::Mouse::getPosition();

			death.checkResumeButton(mouse_coordinates, resume_event);

			death.checkExitButton(mouse_coordinates, resume_event, app_render_window);


			app_render_window.draw(sprite);
			death.draw(app_render_window);
			max_score.draw(app_render_window);
			app_render_window.display();

		}
	}

	max_score.saveItToFile("max_score.txt");

}