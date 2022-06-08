export module label;

import <SFML/Graphics.hpp>;
import <sstream>;
import <fstream>;
import <string>;
import <regex>;

export class Label {

	sf::Text label_score;
	sf::Font font;
	std::string displaying_name;
public:

	int iterator = 0;


	// koordynaty i co wypisac
	Label(int x, int y, std::string str, int it) {
		font.loadFromFile("arial.ttf");
		// zabezpiecz
		displaying_name = str;
		iterator = it;

		label_score.setCharacterSize(25);
		label_score.setPosition(x, y);
		label_score.setFont(font);
		std::ostringstream ssScore;
		ssScore << displaying_name << iterator;
		label_score.setString(ssScore.str());
	}

	void changeLabel(std::string str, int x, int y) {
		label_score.setPosition(x, y);
		std::ostringstream ssScore;
		ssScore << displaying_name << iterator;
		label_score.setString(ssScore.str());
	}

	void updateIterator() {
		std::ostringstream ssScore;
		ssScore << displaying_name << iterator;

		label_score.setString(ssScore.str());
	}
	
	void draw(sf::RenderWindow &rw) {
		rw.draw(label_score); 
	}


	void saveItToFile(std::string filename) {
		std::fstream file;
		file.open(filename);
		if (file.is_open()) {
			file << iterator;

			file.close();
		}
		// catch tutaj
	}

	void getItFromFile(std::string filename) {
		std::string str;
		std::fstream file;
		file.open(filename);
		if (file.is_open()) {
			file >> str;
			
			if (regexmatch(str)) {
				iterator = std::stoi(str);

			}
		}
		file.close();
	}


		// catch tutaj
	

	bool regexmatch(std::string s) {
		std::regex re("\\d+");
		if (regex_match(s, re))
			return true;
		else
			return false;
	}

};