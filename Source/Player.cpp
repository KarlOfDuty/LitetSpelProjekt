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

	birdModel = Model("");
	sharkModel = Model("");
	butterflyModel = Model("");

	this->playerPos = glm::vec3(0.0f, 0.0f, 2.0f);

	setModelMatrix(playerPos);

	//Add characters
	this->playerCharacters[0] = new PlayerBird(100, birdModel);
	this->playerCharacters[1] = new PlayerShark(100, sharkModel);
	this->playerCharacters[2] = new PlayerButterfly(100, butterflyModel);
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

void Player::setModelMatrix(glm::vec3 playerPos)
{
	this->modelMatrix = glm::mat4(
		0.1, 0.0, 0.0, 0.0,
		0.0, 0.1, 0.0, 0.0,
		0.0, 0.0, 0.1, 0.0,
		playerPos.x, playerPos.y, playerPos.z, 1.0
	);

	birdModel.setModelMatrix(modelMatrix);
	sharkModel.setModelMatrix(modelMatrix);
	butterflyModel.setModelMatrix(modelMatrix);
}

//Update funtion
void Player::update(float dt)
{
	//Move
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		playerPos.x - 0.5f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		playerPos.x + 0.5f;
	}
	//Jump
	if (isOnGround)
	{
		PlayerBird *birdPtr = dynamic_cast<PlayerBird*>(playerCharacters[0]);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			dx *= 0.9;

			if (birdPtr != nullptr)
			{
				if (playerCharacters[0]->getJumpAvailable() || birdPtr->getJumpAvailable())
				{
					dy = -10;
					playerCharacters[0]->setJumpAvailable(false);
					birdPtr->setJumpAvailable(false);
				}
			}
			else
			{
				if (playerCharacters[0]->getJumpAvailable())
				{
					dy = -10;
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
		dy += 0.5;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isOnGround && dy > 0)
	{
		dy -= 0.1;
	}

	if (dy > 5)
	{
		dy = 5;
	}

	//Apply velocity
	playerPos.x += dx;
	playerPos.y += dy;

	//Handle collision detection with ground
	//if (playerCollidedWithGround()) {
	//	playerPos.y = positionJustAboveGround;
	//	dy = 0;
	//	isOnGround = true;
	//}

	setModelMatrix(playerPos);
}
//Draws the models involved
void Player::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
	playerCharacters[0]->draw(shader);
}
