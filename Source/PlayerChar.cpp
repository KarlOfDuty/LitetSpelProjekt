#include "PlayerChar.h"

PlayerChar::PlayerChar()
{
	this->HP = 100;
	this->mayJumpAgain = false;

}

PlayerChar::PlayerChar(int HP, Model model)
{
	this->HP = HP;
	this->playerModel = model;
}

PlayerChar::~PlayerChar()
{

}


bool PlayerChar::getJumpAvailable()
{
	return mayJumpAgain;
}

void PlayerChar::setJumpAvailable(bool available)
{
	mayJumpAgain = available;
}

void PlayerChar::takingDamage(int appliedDamage)
{
	this->HP -= appliedDamage;
}

void PlayerChar::setHP(int HP)
{
	this->HP = HP;
}

int PlayerChar::getHP() const
{
	return HP;
}

void PlayerChar::attack()
{

}

void PlayerChar::draw(Shader shader)
{
	playerModel.draw(shader);
}
