#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(int HP, Model model, bool inWater) :PlayerChar(HP, model, inWater)
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
	this->setHP(0);
}
void PlayerButterfly::teleport()
{

}

