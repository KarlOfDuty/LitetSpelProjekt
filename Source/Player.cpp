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

	birdModel = Model("models/cube/cube.obj");
	sharkModel = Model("models/cube/cube.obj");
	butterflyModel = Model("models/cube/cube.obj");

	this->playerPos = glm::vec3(0.0f, 0.0f, 0.0f);

	setModelMatrix(playerPos);

	//Add characters
	this->playerCharacters[0] = new PlayerBird(100, birdModel);
	this->playerCharacters[1] = new PlayerShark(100, sharkModel);
	this->playerCharacters[2] = new PlayerButterfly(100, butterflyModel);
	isOnGround = true;
	this->playerCharacters[0]->setJumpAvailable(true);
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

	birdModel.setModelMatrix(modelMatrix);
	sharkModel.setModelMatrix(modelMatrix);
	butterflyModel.setModelMatrix(modelMatrix);
}

//Update funtion
void Player::update(float dt, int &jumpPress, bool &keyReleased)
{
	groundCheck();
	PlayerBird *birdPtr = dynamic_cast<PlayerBird*>(playerCharacters[0]);

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
		if (jumpPress == 1)
		{
			dx *= 0.9*dt;

			if (playerCharacters[0]->getJumpAvailable())
			{
				dy = 10 * dt;
				birdPtr->setDoubleJump(true);
				this->playerCharacters[0]->setJumpAvailable(false);
			}     
		}
	}

	if (keyReleased == true)
	{
		this->playerCharacters[0]->setJumpAvailable(true);
		keyReleased = false;
	}
	

	if (!isOnGround)
	{
		//If it's the bird character
		if (birdPtr != nullptr)
		{
			//Double jump
			if (birdPtr->getDoubleJump() && jumpPress >= 2)
			{
				dy = 10 * dt;
				birdPtr->setDoubleJump(false);
			}
		}

		//Apply gravity
		dy -= 0.5*dt;
	}

	//Maximum falling speed
	if (dy > 5)
	{
		dy = 5;
	}

	//Apply velocity
	playerPos.x += dx;
	playerPos.y += dy;

	//Handle collision detection with ground
	if (playerPos.y <= 0)
	{
		jumpPress = 0;
		playerPos.y = 0;
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
