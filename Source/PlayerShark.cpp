#include "PlayerShark.h"

PlayerShark::PlayerShark(int HP, Model model) :PlayerChar(HP, model)
{
	this->maxJumps = 1;
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

void PlayerShark::dive()
{

}

