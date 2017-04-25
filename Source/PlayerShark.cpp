#include "PlayerShark.h"

PlayerShark::PlayerShark(int HP, Model model, bool inWater) :PlayerChar(HP, model, inWater)
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

}

