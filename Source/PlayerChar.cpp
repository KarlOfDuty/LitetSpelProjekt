#include "PlayerChar.h"

PlayerChar::PlayerChar()
{
	this->health = 100;
	this->diving = false;
}


PlayerChar::PlayerChar(int health, Model model, bool inWater)
{
	this->health = health;
	this->playerModel = model;
	this->inWater = inWater;
	this->diving = false;
	this->damage = 1;
}

PlayerChar::~PlayerChar()
{

}
void PlayerChar::applyDamage(int appliedDamage)
{
	this->health -= appliedDamage;
}

void PlayerChar::setHealth(int health)
{
	this->health = health;
}

void PlayerChar::setSwim(bool swiming)
{
	this->inWater = swiming;
}

void PlayerChar::setDiving(bool diving)
{
	this->diving = diving;

}

int PlayerChar::getDamage() const
{
	return this->damage;
}

int PlayerChar::getHealth() const
{
	return health;
}
bool PlayerChar::getDiving() const
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