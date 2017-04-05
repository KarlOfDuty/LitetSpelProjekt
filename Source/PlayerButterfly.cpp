#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(int HP) :PlayerChar(HP)
{

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
