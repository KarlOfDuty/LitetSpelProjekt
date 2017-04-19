#include "Player.h"
#include <glm\gtx\matrix_decompose.hpp>
const double PI = 3.14159265358979323846;

void Player::freeMemory()
{
	for (int i = 0; i < 3; i++)
	{
		delete this->playerCharacters[i];
	}
}

Player::Player()
{
	Model* birdModel = new Model("models/Characters/Bird/BirdTest1.obj", modelMatrix);
	Model* sharkModel = new Model("models/sphere/sphere.obj", modelMatrix);
	Model* butterflyModel = new Model("models/cube/cubeGreen.obj", modelMatrix);

	this->playerPos = glm::vec3(0.0f, 2.0f, 0.0f);
	setPos(playerPos);
	this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->modelMatrix *= glm::scale(glm::vec3(0.08f, 0.08f, 0.08f));
	angle = 0;
	this->movementSpeed = 4.0f;
	//Add characters
	this->playerCharacters[0] = new PlayerBird(100, birdModel);
	this->playerCharacters[1] = new PlayerShark(100, sharkModel);
	this->playerCharacters[2] = new PlayerButterfly(100, butterflyModel);
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
	if (player->getMaxJumps() > jumps)
	{
		velocityY = player->getJumpHeight();
		jumps++;
	}
}

void Player::setPos(glm::vec3 playerPos)
{
	this->modelMatrix[3] = glm::vec4(playerPos,1.0f);
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
		goingLeft = true;
		goingRight = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		velocityX = movementSpeed*dt;
		goingRight = true;
		goingLeft = false;
	}
	if (goingLeft == true)
	{
		if (angle != 180)
		{
			this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			angle += 12;
		}
	}

	if (goingRight == true)
	{
		if (angle > 0)
		{
			this->modelMatrix  *= glm::rotate(glm::mat4(), glm::radians(12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			angle -= 12;
		}
		
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
		if (fabs(enemyPos.x - playerPos.x) < 0.1 && fabs(enemyPos.y - playerPos.y) < 1.0)
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
	player->draw(shader);

	for (int i = 0; i < debugCubes.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &debugCubes[i]->getModelMatrix()[0][0]);
		debugCubes[i]->draw(shader);
	}
}
void Player::fixCollision(std::vector<Model*> &allModels)
{
	for (int i = 0; i < 3; i++)
	{
		glm::vec2 mtv(1000, 1000);
		int index = -1;
		float minDistance = 1000;
		/*
		for (int j = 0; j < allModels.size(); j++)
		{
			std::vector<glm::vec2> playerPoints;
			playerPoints.push_back(glm::vec2(-0.5f, -0.0f));
			playerPoints.push_back(glm::vec2(0.5f, -0.0f));
			playerPoints.push_back(glm::vec2(0.5f, 1.0f));
			playerPoints.push_back(glm::vec2(-0.5f, 1.0f));
			std::vector<glm::vec2> objectPoints = allModels[j]->getPoints(allModels[j]->getModelMatrix()[1][1]);
			for (int h = 0; h < playerPoints.size(); h++)
			{
				playerPoints[h] += glm::vec2(playerPos);
				objectPoints[h] += glm::vec2(allModels[j]->getModelMatrix()[3]);
			}
			for (int k = 0; k < playerPoints.size(); k++)
			{
				for (int o = 0; o < objectPoints.size(); o++)
				{
					float distance = glm::length(playerPoints[k] - objectPoints[o]);
					if (distance < minDistance)
					{
						minDistance = distance;
						index = j;
					}
				}
			}
		}
		*/
		
		glm::vec2 player2dPos = glm::vec2(playerPos.x,playerPos.y+0.5f);
		for (int i = 0; i < allModels.size(); i++)
		{
			float distance = glm::length(player2dPos - glm::vec2(allModels[i]->getModelMatrix()[3]));
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
	//std::vector<glm::vec2> playerPoints = player->getModel().getPoints(this->modelMatrix[1][1]);
	std::vector<glm::vec2> playerPoints;
	playerPoints.push_back(glm::vec2(-0.5f,-0.0f));
	playerPoints.push_back(glm::vec2(0.5f, -0.0f));
	playerPoints.push_back(glm::vec2(0.5f, 1.0f));
	playerPoints.push_back(glm::vec2(-0.5f, 1.0f));
	float scaleValue = sqrt(object->getModelMatrix()[0][0] * object->getModelMatrix()[0][0] + object->getModelMatrix()[1][0] * object->getModelMatrix()[1][0] + object->getModelMatrix()[2][0] * object->getModelMatrix()[2][0]);
	std::vector<glm::vec2> objectPoints = object->getPoints(scaleValue);

	glm::mat4 modelMat = object->getModelMatrix();
	glm::quat rotation;
	glm::decompose(modelMat, glm::vec3(), rotation, glm::vec3(), glm::vec3(), glm::vec4());

	double t3 = +2.0 * (rotation.w * rotation.z + rotation.x * rotation.y);
	double t4 = +1.0 - 2.0f * ((rotation.y * rotation.y) + rotation.z * rotation.z);
	float radians = std::atan2(t3, t4);
	std::cout << std::atan2(t3,t4) << std::endl;
	debugCubes.clear();
	for (int i = 0; i < playerPoints.size(); i++)
	{
		playerPoints[i] += glm::vec2(playerPos);
		
		//std::cout << i << " : " << objectPoints[i].x << ", " << objectPoints[i].y << std::endl;
		glm::vec2 center = object->getModelMatrix()[3];
		objectPoints[i] += center;
		objectPoints[i].x = center.x + (objectPoints[i].x - center.x) * cos(radians) - (objectPoints[i].y - center.y) * sin(radians);
		objectPoints[i].y = center.y + (objectPoints[i].x - center.x) * sin(radians) + (objectPoints[i].y - center.y) * cos(radians);
		glm::mat4 modelMat({
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			objectPoints[i].x, objectPoints[i].y, 3.5, 1.0
		});
		debugCubes.push_back(new Model(playerCharacters[2]->getModel(), modelMat));
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