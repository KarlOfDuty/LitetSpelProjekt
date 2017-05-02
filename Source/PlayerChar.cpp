#include "PlayerChar.h"

PlayerChar::PlayerChar()
{
	this->health = 100;
}

PlayerChar::PlayerChar(int health, Model model)
{
	this->health = health;
	this->playerModel = model;
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

int PlayerChar::getDamage() const
{
	return this->damage;
}

int PlayerChar::getHealth() const
{
	return health;
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