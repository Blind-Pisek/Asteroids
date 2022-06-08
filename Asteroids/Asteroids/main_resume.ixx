export module main_resume;

import <SFML/Graphics.hpp>;

import resume;

import window_size;


// modu³ zajmuj¹cy siê resume

export void main_resume(sf::RenderWindow& app_render_window) {

	sf::Texture menu_texture;
	menu_texture.loadFromFile("images/menu/menu_background2_r.jpg");
	menu_texture.setSmooth(true);

	sf::Sprite sprite;
	sprite.setTexture(menu_texture);

	Resume resume(width, height);
	sf::Vector2i mouse_coordinates;


	sf::Event resume_event;

	while (resume.main_loop) {
		while (app_render_window.pollEvent(resume_event)) {

			if (resume_event.type == sf::Event::Closed)
				app_render_window.close();

			if (resume_event.type == sf::Event::KeyPressed) {
				if (resume_event.key.code == sf::Keyboard::Escape) {
					app_render_window.close();
					resume.main_loop = false;
				}
			}
			mouse_coordinates = sf::Mouse::getPosition();

			resume.checkResumeButton(mouse_coordinates, resume_event);

			resume.checkExitButton(mouse_coordinates, resume_event, app_render_window);

			app_render_window.draw(sprite);
			resume.draw(app_render_window);
			app_render_window.display();

		}
	}


}