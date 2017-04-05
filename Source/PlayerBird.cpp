#include "PlayerBird.h"

PlayerBird::PlayerBird(int HP) :PlayerChar(HP)
{

}

PlayerBird::~PlayerBird()
{

}

void PlayerBird::operator=(const PlayerBird &originalObject)
{
	PlayerChar::operator=(originalObject);
}

bool PlayerBird::jumpAllowed()
{
	return false;
}

void PlayerBird::jump()
{
}

void PlayerBird::moveLeft()
{
}

void PlayerBird::moveRight()
{
}

void PlayerBird::attack()
{
}
