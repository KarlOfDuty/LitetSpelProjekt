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

	Model birdModel = Model("models/cube/cube.obj");
	Model sharkModel = Model("models/cube/cube.obj");
	Model butterflyModel = Model("models/cube/cube.obj");

	this->playerPos = glm::vec3(0.0f, 0.0f, 0.0f);

	setModelMatrix(playerPos);

	//Add characters
	this->playerCharacters[0] = new PlayerBird(100, birdModel);
	this->playerCharacters[1] = new PlayerShark(100, sharkModel);
	this->playerCharacters[2] = new PlayerButterfly(100, butterflyModel);
	isOnGround = true;
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

void Player::groundCheck()
{
	if (playerPos.y > 0.0f)
	{
		isOnGround = false;
	}
}

void Player::setModelMatrix(glm::vec3 playerPos)
{
	this->modelMatrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		playerPos.x, playerPos.y, playerPos.z, 1.0
	);
}

//Update funtion
void Player::update(float dt)
{
	groundCheck();

	//Move
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		playerPos.x -= 4.0f*dt;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		playerPos.x += 4.0f*dt;
	}
	//Jump
	if (isOnGround)
	{
		PlayerBird *birdPtr = dynamic_cast<PlayerBird*>(playerCharacters[0]);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			dx *= 0.9*dt;

			if (birdPtr != nullptr)
			{
				if (playerCharacters[0]->getJumpAvailable() || birdPtr->getJumpAvailable())
				{
					dy = 10*dt;
					playerCharacters[0]->setJumpAvailable(false);
					birdPtr->setJumpAvailable(false);

				}
			}
			else
			{
				if (playerCharacters[0]->getJumpAvailable())
				{
					dy = 10*dt;
					playerCharacters[0]->setJumpAvailable(false);
				}
			}

			     
		}
		else
		{
			playerCharacters[0]->setJumpAvailable(true);
			birdPtr->setJumpAvailable(true);
		}
	}

	if (!isOnGround)
	{
		dy -= 0.5*dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isOnGround && dy > 0)
	{
		dy -= 0.1*dt;
	}

	if (dy > 5)
	{
		dy = 5;
	}

	//Apply velocity
	playerPos.x += dx;
	playerPos.y += dy;

	//Handle collision detection with ground
	if (playerPos.y < 0) {
		dy = 0;
		isOnGround = true;
	}

	setModelMatrix(playerPos);
}
//Draws the models involved
void Player::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
	playerCharacters[0]->draw(shader);
}