#include "PlayerBird.h"

PlayerBird::PlayerBird(int health, Model model, bool inWater) :PlayerChar(health, model, inWater)
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

void PlayerBird::waterEffect()
{
	this->setHealth(0);
}