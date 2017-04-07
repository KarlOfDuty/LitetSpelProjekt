#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(int HP, Model model) :PlayerChar(HP, model)
{

}

PlayerButterfly::~PlayerButterfly()
{

}


void PlayerButterfly::operator=(const PlayerButterfly &originalObject)
{
	PlayerChar::operator=(originalObject);
}

void PlayerButterfly::teleport()
{

}

