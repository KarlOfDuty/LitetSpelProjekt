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

	//Add characters
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
	if (charType == 0)
	{
		std::cout << "Characters type is the same as the one currently in use" << std::endl;
	}
	else if (charType == 1 || charType == 2)
	{
		std::swap(playerCharacters[0], playerCharacters[charType]);
	}
}

//Update funtion
void Player::update(float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		this->playerCharacters[0]->moveLeft();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		this->playerCharacters[0]->moveRight();
	}
}
//Draws the models involved
void Player::render(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &cannonModel.getModelMatrix()[0][0]);
	cannonModel.draw(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &cannonModel2.getModelMatrix()[0][0]);
	cannonModel2.draw(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &targetModel.getModelMatrix()[0][0]);
	targetModel.draw(shader);
	if (aCannonBall != nullptr)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &aCannonBall->ballModel->getModelMatrix()[0][0]);
		aCannonBall->ballModel->draw(shader);
	}
}
