#include "PlayerBird.h"

PlayerBird::PlayerBird(int HP, Model model) :PlayerChar(HP, model)
{
	jumpAllowed = true;
}

PlayerBird::~PlayerBird()
{

}

void PlayerBird::setDoubleJump(bool doubleJumpAllowed)
{
	jumpAllowed = doubleJumpAllowed;
}

bool PlayerBird::getDoubleJump()
{
	return jumpAllowed;
}

