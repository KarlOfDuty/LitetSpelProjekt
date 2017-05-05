#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(int health, Model model, bool inWater) :PlayerChar(health, model, inWater)
{
	this->maxJumps = 1;
	this->jumpHeight = 10;
}

PlayerButterfly::~PlayerButterfly()
{

}

void PlayerButterfly::operator=(const PlayerButterfly &originalObject)
{
	PlayerChar::operator=(originalObject);
}

int PlayerButterfly::getMaxJumps()
{
	return maxJumps;
}

float PlayerButterfly::getJumpHeight()
{
	return jumpHeight;
}
void PlayerButterfly::waterEffect()
{
	this->setHealth(0);
}
void PlayerButterfly::teleport()
{

}

