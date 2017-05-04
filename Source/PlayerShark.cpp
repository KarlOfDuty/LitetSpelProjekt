#include "PlayerShark.h"


PlayerShark::PlayerShark(int health, Model model, bool inWater) :PlayerChar(health, model, inWater)
{
	this->maxJumps = 1;
	this->jumpHeight = 10;
}

PlayerShark::~PlayerShark()
{

}

void PlayerShark::operator=(const PlayerShark &originalObject)
{
	PlayerChar::operator=(originalObject);
}

int PlayerShark::getMaxJumps()
{
	return maxJumps;
}

float PlayerShark::getJumpHeight()
{
	return jumpHeight;
}
void PlayerShark::waterEffect()
{
	this->setDiving(true);
}
void PlayerShark::dive()
{
	setDiving(true);
}