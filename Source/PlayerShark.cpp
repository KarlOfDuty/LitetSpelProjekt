#include "PlayerShark.h"

PlayerShark::PlayerShark(int HP, Model model) :PlayerChar(HP, model)
{

}

PlayerShark::~PlayerShark()
{

}


void PlayerShark::operator=(const PlayerShark &originalObject)
{
	PlayerChar::operator=(originalObject);
}

void PlayerShark::dive()
{

}

