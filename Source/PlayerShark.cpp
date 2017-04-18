#include "PlayerShark.h"

PlayerShark::PlayerShark(int HP, Model model) :PlayerChar(HP, model)
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
void PlayerShark::dive()
{

}

