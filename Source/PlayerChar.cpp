#include "PlayerChar.h"

PlayerChar::PlayerChar()
{
	this->HP = 100;
}

PlayerChar::PlayerChar(int HP, Model model)
{
	this->HP = HP;
	playerModel = model;
}

PlayerChar::~PlayerChar()
{

}

PlayerChar::PlayerChar(const PlayerChar & originalObject)
{
	this->HP = originalObject.HP;
}

void PlayerChar::operator=(const PlayerChar &originalObject)
{
	this->HP = originalObject.HP;
}

void PlayerChar::attack()
{

}

void PlayerChar::draw(Shader shader)
{
	playerModel.draw(shader);
}
