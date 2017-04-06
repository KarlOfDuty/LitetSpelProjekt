#include "PlayerBird.h"

PlayerBird::PlayerBird(bool jumpAllowed, int HP, Model model) :PlayerChar(HP, model)
{
	jumpAllowed = true;
}

PlayerBird::~PlayerBird()
{

}

void PlayerBird::operator=(const PlayerBird &originalObject)
{
	PlayerChar::operator=(originalObject);
}

void PlayerBird::setDoubleJump(bool doubleJumpAllowed)
{
	jumpAllowed = doubleJumpAllowed;
}

bool PlayerBird::getDoubleJump()
{
	return jumpAllowed;
}

