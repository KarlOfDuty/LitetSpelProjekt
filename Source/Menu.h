#pragma once
#include "SFML/Graphics.hpp"
#include "SoundSystem.h"

#define MAX_NUMBER_OF_ITEMS 3
#define MAX_NUMBER_OF_OPTIONS 5

class Menu
{
public:
	Menu(float width, float height, SoundSystem * sound);
	~Menu();

	void draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();
	int Select();

private:
	SoundSystem * soundS;
	int activeMenu;
	int selectedItemIndex;
	int selectedOptionIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];
	sf::Text options[MAX_NUMBER_OF_OPTIONS];

};
