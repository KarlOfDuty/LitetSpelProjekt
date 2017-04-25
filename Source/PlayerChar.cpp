#include "PlayerChar.h"

PlayerChar::PlayerChar()
{
	this->HP = 100;
}

PlayerChar::PlayerChar(int HP, Model model, bool inWater)
{
	this->HP = HP;
	this->playerModel = model;
	this->inWater = inWater;
	this->diving = false;
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

void PlayerChar::setSwim(bool swiming)
{
	this->inWater = swiming;
}

void PlayerChar::setDiving(bool diving)
{
	this->diving = diving;

}

int PlayerChar::getHP() const
{
	return HP;
}
int PlayerChar::getDiving() const
{
	return diving;
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