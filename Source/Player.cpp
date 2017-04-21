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
	//GÖR FINAR GÖR WHILE
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
			std::vector<glm::vec2> playerPoints;
			std::vector<glm::vec2> objectPoints;
			float radians = 0.0f;
			getPoints(playerPoints, objectPoints, allModels[index], radians);
			glm::vec2 mtv;
			if (collision::fixCollision(playerPoints, objectPoints, mtv))
			{
				if (radians > 0.0f && radians < 0.79f)
				{
					if (mtv.y > 0)
					{
						playerPos.y -= 0.1f;
					}
					else
					{
						velocityY -= 0.5f;
					}
				}
				else
				{
					playerPos.x += mtv.x;
					if (mtv.y < 0)
					{
						velocityY = 0;
					}
				}
				playerPos.y += mtv.y;
				if (mtv.y > 0)
				{
					if (playerPos.y < 0) playerPos.y = 0;
					groundPos = playerPos.y;
				}
			}
		}
	}
}
void Player::getPoints(std::vector<glm::vec2> &playerPoints, std::vector<glm::vec2> &objectPoints, Model *object, float &radians)
{
	//Set playerPoints
	playerPoints.push_back(glm::vec2(-0.5f, -0.0f));
	playerPoints.push_back(glm::vec2(0.5f, -0.0f));
	playerPoints.push_back(glm::vec2(0.5f, 1.0f));
	playerPoints.push_back(glm::vec2(-0.5f, 1.0f));
	for (int k = 0; k < playerPoints.size(); k++)
	{
		playerPoints[k] += glm::vec2(playerPos);
	}

	//Get rotation and scale from modelMat
	glm::mat4 modelMat = object->getModelMatrix();
	glm::vec3 scale;
	glm::quat rotation;
	glm::decompose(modelMat, scale, rotation, glm::vec3(), glm::vec3(), glm::vec4());

	//Convert from quat to radians
	double t3 = +2.0 * (rotation.w * rotation.z + rotation.x * rotation.y);
	double t4 = +1.0 - 2.0f * ((rotation.y * rotation.y) + rotation.z * rotation.z);
	radians = -std::atan2(t3, t4);

	//Get object points
	objectPoints = object->getPoints(scale);

	//Translate to right position depending on rotation
	for (int k = 0; k < objectPoints.size(); k++)
	{
		glm::vec2 center = object->getModelMatrix()[3];
		objectPoints[k] += center;
		float x = center.x + (objectPoints[k].x - center.x) * cos(radians) - (objectPoints[k].y - center.y) * sin(radians);
		float y = center.y + (objectPoints[k].x - center.x) * sin(radians) + (objectPoints[k].y - center.y) * cos(radians);

		objectPoints[k].x = x;
		objectPoints[k].y = y;
		/*
		glm::mat4 modelMat({
			0.2, 0.0, 0.0, 0.0,
			0.0, 0.2, 0.0, 0.0,
			0.0, 0.0, 0.2, 0.0,
			objectPoints[k].x, objectPoints[k].y, 2.5, 1.0
		});

		if (debugCubes.size() < k + 1)
			debugCubes.push_back(new Model(playerCharacters[1]->getModel(), modelMat));
		else
			debugCubes[k]->setModelMatrix(modelMat);
		*/
	}
}