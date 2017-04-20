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
	this->playerCharacters[0] = new PlayerBird(20, box);
	this->playerCharacters[1] = new PlayerShark(20, box);
	this->playerCharacters[2] = new PlayerButterfly(20, box);
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
	if (this->playerPos.y > 0.0f)
		player = playerCharacters[character];
}

void Player::jump()
{
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

bool Player::playerDead()
{
	if (playerCharacters[0]->getHP() <= 0)
	{
		playerCharacters[0]->setHP(0);

		return true;
	}
	return false;
}

glm::vec3 Player::getPlayerPos() const
{
	return playerPos;
}

//Update function
void Player::update(float dt, std::vector<Model*> &allModels, glm::vec3 enemyPos, int enemyDamage)
{
	groundPos = 0.0f;
	if (playerPos.y > groundPos && isOnGround)
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
	playerPos.x += velocityX;
	velocityX = 0;
	playerPos.y += velocityY*dt;
	
	fixCollision(allModels);

	//Handle collision detection with ground
	if (playerPos.y <= groundPos && !isOnGround)
	{
		jumps = 0;
		if (velocityY < 0)
		{
			playerPos.y = groundPos;
			velocityY = 0;
		}
		isOnGround = true;
	}
	setPos(playerPos);
	//Player taking damage
	if (damageImmunity.getElapsedTime().asSeconds() >= 1.0)
	{
		if (fabs(enemyPos.x - playerPos.x) < 0.2 && fabs(enemyPos.y - playerPos.y) < 1.0)
		{
			playerCharacters[0]->takingDamage(enemyDamage);
			damageImmunity.restart();
		}
	}	
}
//Draws the models involved
void Player::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
	playerCharacters[0]->draw(shader);
}
void Player::fixCollision(std::vector<Model*> &allModels)
{
	for (int i = 0; i < 2; i++)
	{
		glm::vec2 mtv(1000, 1000);
		int index = -1;
		float minDistance = 1000;
		for (int i = 0; i < allModels.size(); i++)
		{
			float distance = glm::length(glm::vec2(playerPos) - glm::vec2(allModels[i]->getModelMatrix()[3]));
			if (distance < minDistance)
			{
				minDistance = distance;
				index = i;
			}
		}
		if (index != -1)
		{
			if (checkCollision(allModels[index], mtv))
			{
				playerPos.x -= mtv.x;
				playerPos.y -= mtv.y;
				if (mtv.y < 0)
				{
					groundPos = playerPos.y;
				}
				else if (mtv.y > 0)
				{
					velocityY = 0;
				}
			}
		}
	}
}
bool Player::checkCollision(Model* object, glm::vec2 &mtv)
{
	std::vector<glm::vec2> playerPoints = player->getModel().getPoints();
	std::vector<glm::vec2> objectPoints = object->getPoints();

	for (int i = 0; i < playerPoints.size(); i++)
	{
		playerPoints[i] += glm::vec2(playerPos);
		objectPoints[i] += glm::vec2(object->getModelMatrix()[3]);
	}

	std::vector<glm::vec2> axis = getAxis(playerPoints, objectPoints);

	std::vector<float> s1;
	std::vector<float> s2;

	for (size_t i = 0; i < axis.size(); i++)
	{
		s1.clear();
		s2.clear();
		for (size_t j = 0; j < playerPoints.size(); j++)
		{
			for (size_t k = 0; k < j; k++)
			{
				s1.push_back(glm::dot(playerPoints[k], axis[i]));
				s2.push_back(glm::dot(objectPoints[k], axis[i]));
			}
		}

		float s1min = s1[0];
		float s1max = s1[0];
		float s2min = s2[0];
		float s2max = s2[0];
		for (size_t x = 1; x < s1.size(); x++)
		{
			if (s1min > s1[x]) s1min = s1[x];
			if (s1max < s1[x]) s1max = s1[x];
			if (s2min > s2[x]) s2min = s2[x];
			if (s2max < s2[x]) s2max = s2[x];
		}
		if (s2min > s1max || s2max < s1min) 
			return false;

		float overlap;
		if (s1min < s2min)
			overlap = s1max - s2min;
		else 
			overlap = -(s2max - s1min);

		if (abs(overlap) < length(mtv))
			mtv = axis[i] * overlap;
	}

	return true;
}
std::vector<glm::vec2> Player::getAxis(std::vector<glm::vec2> points1, std::vector<glm::vec2> points2)
{
	std::vector<glm::vec2> axis;
	axis.push_back(glm::normalize(points1[1] - points1[0]));
	axis.push_back(glm::normalize(points1[3] - points1[0]));
	axis.push_back(glm::normalize(points2[1] - points2[0]));
	axis.push_back(glm::normalize(points2[3] - points2[0]));
	return axis;
}