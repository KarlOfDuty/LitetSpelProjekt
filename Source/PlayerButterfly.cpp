#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(int health, Model model) :PlayerChar(health, model)
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
void PlayerButterfly::teleport()
{

}

