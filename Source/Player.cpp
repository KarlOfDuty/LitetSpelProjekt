#include "Player.h"

void Player::freeMemory()
{
	for (int i = 0; i < 3; i++)
	{
		delete this->playerCharacters[i];
	}
}

Player::Player()
{
	Model *box = new Model("models/cube/cube.obj");

	this->playerPos = glm::vec3(0.0f, 2.0f, 0.0f);

	setPos(playerPos);

	this->movementSpeed = 4.0f;
	//Add characters
	this->playerCharacters[0] = new PlayerBird(100, box);
	this->playerCharacters[1] = new PlayerShark(100, box);
	this->playerCharacters[2] = new PlayerButterfly(100, box);
	this->player = playerCharacters[0];
	this->isOnGround = true;
}

Player::~Player()
{
	this->freeMemory();
}

glm::vec3 Player::getPlayerPos()
{
	return this->playerPos;
}

void Player::swap(int character)
{
	player = playerCharacters[character];
}

void Player::jump()
{
	if(jumps == 0)
		deltaClock.restart();
	if (player->getMaxJumps() > jumps)
	{

		velocityY = player->getJumpHeight();
		jumps++;
	}
}

void Player::setPos(glm::vec3 playerPos)
{
	this->modelMatrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		playerPos.x, playerPos.y, playerPos.z, 1.0
	);
}

//Update funtion
void Player::update(float dt, sf::Window &window, std::vector<Model*> &allModels)
{
	groundPos = 0;
	velocityX = 0;
	if (playerPos.y > 0.0f && isOnGround)
	{
		isOnGround = false;
	}
	int controller = CONTROLLER0;
	//Move
	if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) < -20)
	{
		velocityX = -movementSpeed*dt;
	}
	else if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) > 20)
	{
		velocityX = movementSpeed*dt;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		velocityX = -movementSpeed*dt;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		velocityX = movementSpeed*dt;
	}
	//If in air
	if (!isOnGround)
	{
		velocityY -= 30*dt;
	}

	//Maximum falling speed
	if (velocityY < -30)
	{
		velocityY = -30;
	}

	//Apply velocity
	playerPos.y += velocityY;
	int amountOfTries = 0;
	while (fixCollision(allModels) && amountOfTries < 5)
	{
		amountOfTries++;
	}
	playerPos.x += velocityX;
	playerPos.y += velocityY*dt;

	//Handle collision detection with ground
	if (playerPos.y <= 0 && !isOnGround)
	{
		float testTime = deltaClock.restart().asSeconds();
		std::cout << testTime << std::endl;
		jumps = 0;
		playerPos.y = 0;
		velocityY = 0;
		isOnGround = true;
	}
	setPos(playerPos);
}
//Draws the models involved
void Player::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
	playerCharacters[0]->draw(shader);
}

bool Player::collidesWith(Model* object)
{
	glm::vec3 minPlayer = -0.5f + this->playerPos;
	glm::vec3 maxPlayer = 0.5f + this->playerPos;

	glm::vec3 objectPos = object->getModelMatrix()[3];
	glm::vec3 minObject = -0.5f + objectPos;
	glm::vec3 maxObject = 0.5f + objectPos;

	if (maxPlayer.x > minObject.x &&
		minPlayer.x < maxObject.x &&
		maxPlayer.y > minObject.y &&
		minPlayer.y < maxObject.y &&
		maxPlayer.z > minObject.z &&
		minPlayer.z < maxObject.z)
	{
		return true;
	}

	return false;
}
bool Player::fixCollision(std::vector<Model*> allModels)
{
	int index = -1;
	float minDist = 1000.0f;
	for (int i = 0; i < allModels.size(); i++)
	{
		if (collidesWith(allModels[i]))
		{
			float distanceToModel = glm::distance(glm::vec3(allModels[i]->getModelMatrix()[3]), playerPos);
			if (distanceToModel < minDist)
			{
				index = i;
				minDist = distanceToModel;
			}
		}
	}
	if (index != -1)
	{
		float xLen = fabs(allModels[index]->getModelMatrix()[3].x - playerPos.x) - 1.0f;
		float yLen = fabs(allModels[index]->getModelMatrix()[3].y - playerPos.y) - 1.0f;

		if (xLen > yLen)
		{
			if (velocityX > 0)
			{
				this->playerPos.x += xLen;
			}
			else
			{
				this->playerPos.x -= xLen;
			}
			velocityX = 0;
		}
		else
		{
			if (velocityY > 0)
			{
				this->playerPos.y += yLen;
				velocityY = 0;
			}
			else if (velocityY < 0)
			{
				this->playerPos.y -= yLen;
				groundPos = playerPos.y;
				velocityY = 0;
			}
		}
	}
	return index != -1;
}