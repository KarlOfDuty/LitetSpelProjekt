#include "PlayerBird.h"

PlayerBird::PlayerBird(int health, Model model) :PlayerChar(health, model)
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