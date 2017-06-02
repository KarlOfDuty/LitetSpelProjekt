#include "PlayerChar.h"

PlayerChar::PlayerChar()
{

}


PlayerChar::PlayerChar(Model* model, bool inWater)
{
	this->playerModel = model;
	this->inWater = inWater;
	this->damage = 1;
}

PlayerChar::~PlayerChar()
{

}

void PlayerChar::setSwim(bool swiming)
{
	this->inWater = swiming;
}

int PlayerChar::getDamage() const
{
	return this->damage;
}
void PlayerChar::update(float dt)
{
	playerModel->updateAnimation(dt);
}
void PlayerChar::draw(Shader shader)
{
	playerModel->draw(shader);
}

Model* PlayerChar::getModel()
{
	return this->playerModel;
}