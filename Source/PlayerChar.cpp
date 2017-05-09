#include "PlayerChar.h"

PlayerChar::PlayerChar()
{
	this->diving = false;
}


PlayerChar::PlayerChar(Model model, bool inWater)
{
	this->playerModel = model;
	this->inWater = inWater;
	this->diving = false;
	this->damage = 1;
}

PlayerChar::~PlayerChar()
{

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

bool PlayerChar::getDiving() const
{
	return diving;
}

void PlayerChar::draw(Shader shader)
{
	playerModel.draw(shader);
}

Model& PlayerChar::getModel()
{
	return this->playerModel;
}