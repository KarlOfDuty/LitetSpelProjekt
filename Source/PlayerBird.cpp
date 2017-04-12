#include "PlayerBird.h"

PlayerBird::PlayerBird(int HP, Model model) :PlayerChar(HP, model)
{
	this->maxJumps = 2;
	this->jumpHeight = 0.3f;
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