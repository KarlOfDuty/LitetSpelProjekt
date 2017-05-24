#ifndef GUI_H
#define GUI_H

#include <SFML\Graphics.hpp>
#include "Player.h"
#include "EnemyManager.h"

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
	bool bossIsKilled;
	sf::Clock bossTextDelay;
	sf::Text bossText;
public:
	GUI();
	~GUI();
	void update(Player* player, EnemyManager* enemy);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif // !GUdI_H