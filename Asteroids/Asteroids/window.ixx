export module window;


import <iostream>;
import <string>;
import <sstream>;
import <iomanip>;

import <SFML/Graphics.hpp>;


// wersja gry
const float version = 1.9;

// wymiary okna gry
const int width = 1400;
const int height = 900;

export std::string windowName() {
	std::string app_name = "Symulator lotow z asteroidami!";
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << version;
	std::string s = stream.str();
	std::string app_version = " Patch: " + s;

	return app_name + app_version;
}


export void windowSettings(sf::RenderWindow& app_render_window) {
	app_render_window.create(sf::VideoMode(width, height), windowName());
	app_render_window.setFramerateLimit(120);

	app_render_window.setVerticalSyncEnabled(true);
	app_render_window.setKeyRepeatEnabled(false);
}

