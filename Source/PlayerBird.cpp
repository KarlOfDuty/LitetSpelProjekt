#include "PlayerBird.h"

PlayerBird::PlayerBird(Model model, bool inWater) :PlayerChar(model, inWater)
{
	this->maxJumps = 2;
	this->jumpHeight = 15;
}

PlayerBird::~PlayerBird()
{

}
void PlayerBird::operator=(const PlayerBird &originalObject)
{
	PlayerChar::operator=(originalObject);
}

int PlayerBird::getMaxJumps()
{
	return maxJumps;
}

float PlayerBird::getJumpHeight()
{
	return jumpHeight;
}