#include "GUI.h"



GUI::GUI()
{
	isDead = false;
	deadBox = sf::RectangleShape(sf::Vector2f(1280, 200));
	deadBox.setFillColor(sf::Color(0, 0, 0, 0));
	deadBox.setOrigin(1280/2, 100);
	deadBox.setPosition(1280 / 2, 720 / 2);
	deadBox.setOutlineColor(sf::Color(40, 40, 40, 0));
	deadBox.setOutlineThickness(5);
	if (!deadFont.loadFromFile("fonts/font.ttf"))
	{
		std::cout << "FAILED LOAD" << std::endl;
	}
	deadText.setFont(deadFont);
	deadText.setString("YOU DIED");
	deadText.setCharacterSize(100);
	deadText.setFillColor(sf::Color(200,20,20,0));
	deadText.setOrigin(deadText.getGlobalBounds().left + round(deadText.getGlobalBounds().width / 2), deadText.getGlobalBounds().top + round(deadText.getGlobalBounds().height / 2));
	deadText.setPosition(1280 / 2, 720 / 2);

	heartLeftImage.loadFromFile("sprites/leftHeart.png");
	heartRightImage.loadFromFile("sprites/rightHeart.png");

}


GUI::~GUI()
{

}

void GUI::update(Player * player)
{
	healthBar.clear();
	for (int i = 0; i < player->getHealth(); i++)
	{
		sf::Sprite temp;
		if (i%2 == 0)
		{
			temp.setTexture(heartLeftImage);
			temp.setPosition(i * 16 +16, 16);
		}
		else
		{
			temp.setTexture(heartRightImage);
			temp.setPosition((i-1) * 16 + 32, 16);
		}
		healthBar.push_back(temp);
	}
	if (player->getHealth() <= 0)
	{
		isDead = true;
	}
	if (isDead)
	{
		sf::Color newColor = deadBox.getFillColor();
		sf::Color newOutlineColor = deadBox.getOutlineColor();
		sf::Color newTextColor = deadText.getFillColor();
		if (newColor.a + 3 > 255)
		{
			newColor.a = 255;
			newOutlineColor.a = 255;
			newTextColor.a = 255;
		}
		else
		{
			newColor.a += 3;
			newOutlineColor.a += 3;
			newTextColor.a += 3;
		}
		deadBox.setFillColor(newColor);
		deadBox.setOutlineColor(newOutlineColor);
		deadText.setFillColor(newTextColor);
	}
}

void GUI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (int i = 0; i < healthBar.size(); i++)
	{
		target.draw(healthBar[i]);
	}
	target.draw(deadBox);
	target.draw(deadText);
}