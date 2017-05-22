#ifndef GUI_H
#define GUI_H

#include <SFML\Graphics.hpp>
#include "Player.h"

class GUI : public sf::Drawable
{
private:
	std::vector<sf::Sprite> healthBar;
	sf::Texture heartLeftImage;
	sf::Texture heartRightImage;
	bool isDead;
	sf::RectangleShape deadBox;
	sf::Text deadText;
	sf::Font deadFont;
public:
	GUI();
	~GUI();
	void update(Player* player);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif // !GUdI_H