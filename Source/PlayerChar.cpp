#include "PlayerChar.h"

PlayerChar::PlayerChar()
{
	this->HP = 100;
}

PlayerChar::PlayerChar(int HP, Model model)
{
	this->HP = HP;
	this->playerModel = model;
}

PlayerChar::~PlayerChar()
{

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

Model& PlayerChar::getModel()
{
	return this->playerModel;
}