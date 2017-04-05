#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(int HP) :PlayerChar(HP)
{
	Model butterflyModel1 = Model("", {
		0.1, 0.0, 0.0, 0.0,
		0.0, 0.1, 0.0, 0.0,
		0.0, 0.0, 0.1, 0.0,
		2.0, -0.12, 2.0, 1.0 });

	this->butterflyModel = Model(butterflyModel1);
}

PlayerButterfly::~PlayerButterfly()
{

}

void PlayerButterfly::operator=(const PlayerButterfly &originalObject)
{
	PlayerChar::operator=(originalObject);
}

void PlayerButterfly::teleport()
{

}

void PlayerButterfly::jump()
{
}

void PlayerButterfly::moveLeft()
{
}

void PlayerButterfly::moveRight()
{
}

void PlayerButterfly::attack()
{
}
