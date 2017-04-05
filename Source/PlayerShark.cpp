#include "PlayerShark.h"

PlayerShark::PlayerShark(int HP) :PlayerChar(HP)
{
	Model sharkModel1 = Model("", {
		0.1, 0.0, 0.0, 0.0,
		0.0, 0.1, 0.0, 0.0,
		0.0, 0.0, 0.1, 0.0,
		2.0, -0.12, 2.0, 1.0 });

	this->sharkModel = Model(sharkModel1);
}

PlayerShark::~PlayerShark()
{

}

void PlayerShark::operator=(const PlayerShark &originalObject)
{
	PlayerChar::operator=(originalObject);
}

void PlayerShark::dive()
{

}

void PlayerShark::jump()
{
}

void PlayerShark::moveLeft()
{
}

void PlayerShark::moveRight()
{
}

void PlayerShark::attack()
{
}
