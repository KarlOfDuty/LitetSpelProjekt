#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(int HP, Model model) :PlayerChar(HP, model)
{
	this->maxJumps = 1;
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

void PlayerButterfly::teleport()
{

}

