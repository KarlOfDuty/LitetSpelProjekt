#include "Menu.h"

Menu::Menu(float width, float height, SoundSystem * sound)
{
	activeMenu = 0;

	soundS = sound;

	if (!font.loadFromFile("arial.ttf"))
	{
		//handel error
	}
	//main menu
	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("Play");
	menu[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("Options");
	menu[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("Exit");
	menu[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

	selectedItemIndex = 0;

	//options
	options[0].setFont(font);
	options[0].setFillColor(sf::Color::Red);
	options[0].setString("Master Volume: On");
	options[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_OPTIONS + 1) * 1));

	options[1].setFont(font);
	options[1].setFillColor(sf::Color::White);
	options[1].setString("Music: On");
	options[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_OPTIONS + 1) * 2));

	options[2].setFont(font);
	options[2].setFillColor(sf::Color::White);
	options[2].setString("Sound Effects: On");
	options[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_OPTIONS + 1) * 3));

	options[3].setFont(font);
	options[3].setFillColor(sf::Color::White);
	options[3].setString("Place holder for graphics");
	options[3].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_OPTIONS + 1) * 4));

	options[4].setFont(font);
	options[4].setFillColor(sf::Color::White);
	options[4].setString("Back");
	options[4].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_OPTIONS + 1) * 5));


	selectedOptionIndex = 0;

}

Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow & window)
{
	if (activeMenu == 0)
	{
		for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
		{
			window.draw(menu[i]);
		}
	}
	else if(activeMenu == 1)
	{
		for (int i = 0; i < MAX_NUMBER_OF_OPTIONS; i++)
		{
			window.draw(options[i]);
		}
	}
	
}

void Menu::MoveUp()
{
	if (activeMenu == 0)
	{
		if (selectedItemIndex - 1 >= 0)
		{
			menu[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex--;
			menu[selectedItemIndex].setFillColor(sf::Color::Red);
		}
	}
	else if(activeMenu == 1)
	{
		if (selectedOptionIndex - 1 >= 0)
		{
			options[selectedOptionIndex].setFillColor(sf::Color::White);
			selectedOptionIndex--;
			options[selectedOptionIndex].setFillColor(sf::Color::Red);
		}
	}
	
}

void Menu::MoveDown()
{
	if (activeMenu == 0)
	{
		if (selectedItemIndex < MAX_NUMBER_OF_ITEMS - 1)
		{
			menu[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex++;
			menu[selectedItemIndex].setFillColor(sf::Color::Red);
		}
	}
	else if (activeMenu == 1)
	{
		if (selectedOptionIndex < MAX_NUMBER_OF_OPTIONS - 1)
		{
			options[selectedOptionIndex].setFillColor(sf::Color::White);
			selectedOptionIndex++;
			options[selectedOptionIndex].setFillColor(sf::Color::Red);
		}
	}
}

int Menu::Select()
{
	int result = -1;
	if (activeMenu == 0)
	{
		if (selectedItemIndex == 0)
		{
			//start game loop
			result = 0;
		}
		else if (selectedItemIndex == 1)
		{
			activeMenu = 1;
			result = 1;
		}
		else if (selectedItemIndex == 2)
		{
			//exit
			result = 2;
		}
	}
	else if (activeMenu == 1)
	{
		if (selectedOptionIndex == 0)
		{
			if (soundS->getSoundEnabled() == true)
			{
				soundS->stopSound();
				soundS->setSoundEnabled(false);
				options[selectedOptionIndex].setString("Master Volume: Off");
			}
			else
			{
				soundS->setSoundEnabled(true);
				soundS->startSound();
				options[selectedOptionIndex].setString("Master Volume: On");
			}

		}
		else if (selectedOptionIndex == 1)
		{

			if (soundS->getMusicEnabled() == true)
			{
				soundS->stopMusic();
				soundS->setMusicEnabled(false);
				options[selectedOptionIndex].setString("Music: Off");
			}
			else
			{
				soundS->setMusicEnabled(true);
				soundS->startMusic();
				options[selectedOptionIndex].setString("Music: On");
			}
		}

		else if (selectedOptionIndex == 2)
		{
			if (soundS->getSfxEnabled() == true)
			{
				soundS->stopSfx();
				soundS->setSfxEnabled(false);
				options[selectedOptionIndex].setString("Sound Effects: Off");
			}
			else
			{
				soundS->setSfxEnabled(true);
				soundS->startSfx();
				options[selectedOptionIndex].setString("Sound Effects: On");
			}
		}

		else if (selectedOptionIndex == 3)
		{

		}

		else if (selectedOptionIndex == 4)
		{
			activeMenu = 0;
		}
	}
	
	
	return result;
}
