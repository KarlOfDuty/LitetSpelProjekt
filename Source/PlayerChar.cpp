#include "PlayerChar.h"

PlayerChar::PlayerChar()
{
	this->HP = 100;
}

PlayerChar::PlayerChar(int HP)
{
	this->HP = HP;
}

PlayerChar::~PlayerChar()
{

}

void PlayerChar::operator=(const PlayerChar &originalObject)
{
	this->HP = originalObject.HP;
}

void PlayerChar::jump()
{

}

void PlayerChar::moveLeft()
{

}

void PlayerChar::moveRight()
{

}

void PlayerChar::attack()
{

}
