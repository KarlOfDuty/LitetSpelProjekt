#include "PlayerChar.h"

PlayerChar::PlayerChar()
{
	this->HP = 100;
	this->mayJumpAgain = false;

}

PlayerChar::PlayerChar(int HP, Model model)
{
	this->HP = HP;
	playerModel = model;
}

PlayerChar::~PlayerChar()
{

}

PlayerChar::PlayerChar(const PlayerChar & originalObject)
{
	this->HP = originalObject.HP;
}

void PlayerChar::operator=(const PlayerChar &originalObject)
{
	this->HP = originalObject.HP;
}

bool PlayerChar::getJumpAvailable()
{
	return mayJumpAgain;
}

void PlayerChar::setJumpAvailable(bool available)
{
	mayJumpAgain = available;
}

void PlayerChar::attack()
{

}

void PlayerChar::draw(Shader shader)
{
	playerModel.draw(shader);
}
