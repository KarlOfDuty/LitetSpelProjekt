#include "Player.h"

void Player::initiate()
{
	for (int i = 0; i < 2; i++)
	{
		this->playerCharacters[i] = nullptr;
	}
}

void Player::freeMemory()
{
	for (int i = 0; i < 2; i++)
	{
		delete this->playerCharacters[i];
	}
	delete[] this->playerCharacters;
}

Player::Player()
{
	this->playerCharacters = new PlayerChar*[2];
	this->initiate();

	this->playerCharacters[0] = new PlayerBird(100);
	this->playerCharacters[1] = new PlayerShark(100);
	this->playerCharacters[2] = new PlayerButterfly(100);
}

Player::~Player()
{
	this->freeMemory();
}

void Player::swap(int charType)
{
	if(charType ==)
}
