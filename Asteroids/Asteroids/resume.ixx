export module resume;

import <SFML/Graphics.hpp>;
import <sstream>;

import menu;

const int size = 3;

export class Resume : public Menu {

	void createNameButton() {
		menu[0].setFont(font);
		menu[0].setFillColor(sf::Color::White);
		menu[0].setString("Game paused");
		menu[0].setCharacterSize(50);
		menu[0].setPosition(600, 150);
	}
	void createResumeButton(float width, float height) {
		menu[1].setFont(font);
		menu[1].setFillColor(sf::Color::White);
		menu[1].setString("Resume");
		menu[1].setCharacterSize(50);
		menu[1].setPosition((width / 2) - 30, (height / 2) + 50);
	}
	void createExitButton(float width, float height) {
		menu[2].setFont(font);
		menu[2].setFillColor(sf::Color::White);
		menu[2].setString("Exit");
		menu[2].setCharacterSize(50);
		menu[2].setPosition((width / 2) - 30, (height / 2) + 230);
	}

public:

	Resume(float width, float height) {
		main_loop = true;

		if (!font.loadFromFile("arial.ttf")) {
			// handle error
			// catch
		}
		createNameButton();
		createResumeButton(width, height);
		createExitButton(width, height);
	}

	void draw(sf::RenderWindow& window) const {
		for (int i = 0; i < size; i++)
			window.draw(menu[i]);
	}

	void checkResumeButton(sf::Vector2i& vec, sf::Event menu_event) {
		if (vec.x >= 900 and vec.x <= 1060 and vec.y >= 625 and vec.y <= 675) {
			menu[1].setFillColor(sf::Color::Cyan);
			if (menu_event.type == sf::Event::MouseButtonPressed) {
				if (menu_event.key.code == sf::Mouse::Left)
					main_loop = false;
			}
		}
		else
			menu[1].setFillColor(sf::Color::White);
	}

	void checkExitButton(sf::Vector2i& vec, sf::Event menu_event, sf::RenderWindow& app_render_window) {
		if (vec.x >= 900 and vec.x <= 1060 and vec.y >= 805 and vec.y <= 855) {
			menu[2].setFillColor(sf::Color::Red);
			if (menu_event.type == sf::Event::MouseButtonPressed or menu_event.type == sf::Event::KeyPressed) {
				if (menu_event.key.code == sf::Mouse::Left or menu_event.key.code == sf::Keyboard::Escape) {
					main_loop = false;
					app_render_window.close();
				}

			}
		}
		else
			menu[2].setFillColor(sf::Color::White);
	}

};