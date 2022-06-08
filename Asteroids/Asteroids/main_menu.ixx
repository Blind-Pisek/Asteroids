export module main_menu;

import <SFML/Graphics.hpp>;

import menu;

import window_size;


// modu³ zajmuj¹cy siê obs³ug¹ menu

export void main_menu(sf::RenderWindow &app_render_window) {
	// tutaj sie bawie menu
	sf::Texture menu_texture;
	menu_texture.loadFromFile("images/menu/menu_background2_r.jpg");
	menu_texture.setSmooth(true);

	sf::Sprite sprite;
	sprite.setTexture(menu_texture);

	Menu menu(width, height);
	sf::Event menu_event;
	
	sf::Vector2i mouse_coordinates;

	while (menu.main_loop) {
		while (app_render_window.pollEvent(menu_event)) {

			if (menu_event.type == sf::Event::Closed)
				app_render_window.close();

			if (menu_event.type == sf::Event::KeyPressed) {
				if (menu_event.key.code == sf::Keyboard::Escape) {
					// narazie zamykam gre
					app_render_window.close();
					menu.main_loop = false;
				}
			}

			mouse_coordinates = sf::Mouse::getPosition();

			menu.checkPlayButton(mouse_coordinates, menu_event);

			menu.checkExitButton(mouse_coordinates, menu_event, app_render_window);

			app_render_window.draw(sprite);
			menu.draw(app_render_window);
			app_render_window.display();
		}
	}
}