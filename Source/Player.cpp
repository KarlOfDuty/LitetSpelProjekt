#include "Player.h"
#include "Enemy.h"
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
	Model* sharkModel = new Model("models/Characters/Shark/Fish_T-Pose_Export.obj", modelMatrix);
	Model* butterflyModel = new Model("models/Characters/Butterfly/ButterFly.obj", modelMatrix);

	arrow = new Model("models/cube/cubeGreen.obj");

	this->modelMatrix[3] = glm::vec4(0.0f, 2.0f, 0.0f, 1.0);
	this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->modelMatrix *= glm::scale(glm::vec3(1, 1, 1));

	angle = 0;
	this->movementSpeed = 100.0f;
	//Add characters
	this->health = 20;

	this->playerCharacters[0] = new PlayerBird(birdModel, false);
	this->playerCharacters[1] = new PlayerShark(sharkModel, false);
	this->playerCharacters[2] = new PlayerButterfly(butterflyModel, false);
	this->player = playerCharacters[0];
	this->isOnGround = true;

	allAttackBoxes = std::vector<Projectile*>();
	allArrowAttackBoxes = std::vector<Projectile*>();
	allAOEAttackBoxes = std::vector<Projectile*>();
	allMeleeAttackBoxes = std::vector<Projectile*>();
}

Player::~Player()
{
	this->freeMemory();
}

PlayerChar* Player::getCurrentCharacter()
{
	return player;
}

std::vector<Projectile*> Player::getProjectiles()
{
	return allAttackBoxes;
}

Projectile* Player::getProjectileAt(int nr)
{
	return allAttackBoxes[nr];
}

int Player::getDamage() const
{
	return playerCharacters[0]->getDamage();
}

int Player::getHealth() const
{
	return health;
}

std::vector<glm::vec2> Player::getPoints()
{
	std::vector<glm::vec2> playerPoints;
	playerPoints.push_back(glm::vec2(-0.5f, -0.0f));
	playerPoints.push_back(glm::vec2(0.5f, -0.0f));
	playerPoints.push_back(glm::vec2(0.5f, 1.0f));
	playerPoints.push_back(glm::vec2(-0.5f, 1.0f));
	for (int k = 0; k < playerPoints.size(); k++)
	{
		playerPoints[k] += glm::vec2(getPos());
	}
	return playerPoints;
}
//Swaps the current player
void Player::swap(int character)
{
	player = playerCharacters[character];
}

//Makes the player jump
void Player::jump()
{
	if(this->diving == false)
	{	if (player->getMaxJumps() > jumps)
		{
			velocityY = player->getJumpHeight();
			jumps++;
		}
	}
	else
	{
		if (player->getMaxJumps() > jumps)
		{
			velocityY = player->getJumpHeight();
		}
	}
}


void Player::waterEffect()
{
	if (this->player == playerCharacters[2])
	{
		setHealth(0);
	}
	else if (this->player == playerCharacters[0])
	{
		setHealth(0);
	}
}

void Player::applyDamage(int appliedDamage)
{
	if (this->damageImmunity.getElapsedTime().asSeconds() >= 1.2)
	{
		this->health -= appliedDamage;
		this->damageImmunity.restart();
	}
}

void Player::lightAttackPressed(sf::Window &window)
{
	PlayerButterfly* butterfly = dynamic_cast<PlayerButterfly*>(player);
	if (butterfly != nullptr)
	{
		int mouseX = sf::Mouse::getPosition(window).x;
		int middleScreenX = window.getSize().x / 2;
		glm::vec2 position = (mouseX >= middleScreenX) ? glm::vec2(getPos().x + 3.0f, getPos().y) : glm::vec2(getPos().x - 3.0f, getPos().y);
		butterfly->shootAoe(allStaticModels, allAOEAttackBoxes, position);
	}
	PlayerBird* bird = dynamic_cast<PlayerBird*>(player);
	if (bird != nullptr)
	{
		int mouseX = sf::Mouse::getPosition(window).x;
		int middleScreenX = window.getSize().x / 2;
		glm::vec2 position;
		glm::vec2 direction;
		if (mouseX >= middleScreenX)
		{
			position = glm::vec2(getPos().x + 1.0f, getPos().y);
			direction = glm::vec2(1, 0);
		}
		else
		{
			position = glm::vec2(getPos().x - 1.0f, getPos().y);
			direction = glm::vec2(-1, 0);
		}
		bird->meleeAttack(allMeleeAttackBoxes, position, direction, 20.f);
	}
}
void Player::lightAttackReleased(sf::Window &window)
{
	PlayerShark* bird = dynamic_cast<PlayerShark*>(player);
	if (bird != nullptr && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		//Get direction and scale
		glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
		float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
		glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));
		glm::vec2 startPos = glm::vec2(getPos().x, getPos().y + 2.0f);
		bird->shootArrow(allArrowAttackBoxes, startPos, direction);
	}
}
void Player::heavyAttackPressed(sf::Window &window)
{
	PlayerButterfly* butterfly = dynamic_cast<PlayerButterfly*>(player);
	if (butterfly != nullptr)
	{
		int mouseX = sf::Mouse::getPosition(window).x;
		int middleScreenX = window.getSize().x / 2;
		glm::vec2 position = (mouseX >= middleScreenX) ? glm::vec2(getPos().x + 10.0f, getPos().y) : glm::vec2(getPos().x - 10.0f, getPos().y);
		butterfly->shootAoe(allStaticModels, allAOEAttackBoxes, position);
	}
	PlayerBird* bird = dynamic_cast<PlayerBird*>(player);
	if (bird != nullptr)
	{
		int mouseX = sf::Mouse::getPosition(window).x;
		int middleScreenX = window.getSize().x / 2;
		glm::vec2 position;
		glm::vec2 direction;
		if (mouseX >= middleScreenX)
		{
			position = glm::vec2(getPos().x + 1.0f, getPos().y);
			direction = glm::vec2(1, 0);
		}
		else
		{
			position = glm::vec2(getPos().x - 1.0f, getPos().y);
			direction = glm::vec2(-1, 0);
		}
		bird->meleeAttack(allMeleeAttackBoxes, position, direction, 1.f);
	}
	PlayerShark* shark = dynamic_cast<PlayerShark*>(player);
	if (shark != nullptr)
	{
		shark->arrowVelocity = 5.f;
	}
}
void Player::heavyAttackReleased(sf::Window &window)
{
	PlayerShark* bird = dynamic_cast<PlayerShark*>(player);
	if (bird != nullptr)
	{
		//Get direction and scale
		glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
		float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
		glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));
		glm::vec2 startPos = glm::vec2(getPos().x, getPos().y + 2.0f);
		bird->shootArrow(allArrowAttackBoxes, startPos, direction);
		bird->arrowVelocity = 30.0f;
	}
}

void Player::clearProjectiles()
{
	for (int i = 0; i < allAttackBoxes.size(); i++)
	{
		allAttackBoxes[i]->disableArrow();
	}
}

void Player::aiming(sf::Window &window,float dt)
{
	PlayerShark* bird = dynamic_cast<PlayerShark*>(player);
	if (bird != nullptr)
	{
		if (bird->arrowVelocity >= 60.0f)
		{
			bird->arrowVelocity = 60.0f;
		}
		else
		{
			bird->arrowVelocity += 20.0f * dt;
		}

		glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
		float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
		glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));

		glm::vec2 position = glm::vec2(getPos().x, getPos().y + 2.f);
		glm::vec2 velocity = glm::vec2(glm::abs(direction.x*bird->arrowVelocity), direction.y*bird->arrowVelocity);
		for (int i = 0; i < 30; i++)
		{
			velocity.x -= 5.0f*0.02f;
			if (velocity.x < 0) velocity.x = 0;
			velocity.y -= 30.0f*0.02f;
			position.x += direction.x*velocity.x*0.02f;
			position.y += velocity.y*0.02f;

			glm::mat4 modelMat({
				0.1, 0.0, 0.0, 0.0,
				0.0, 0.1, 0.0, 0.0,
				0.0, 0.0, 0.1, 0.0,
				position.x, position.y , 0.0, 1.0
			});

			if (debugCubes.size() <= i)
			{
				debugCubes.push_back(new Model(arrow, modelMat));
			}
			else
			{
				debugCubes[i]->setModelMatrix(modelMat);
			}
		}
	}
}

//Sets the playerPos variable
void Player::setPos(glm::vec3 playerPos)
{
	this->modelMatrix[3] = glm::vec4(playerPos,1.0);
}

//True if player is dead
bool Player::playerIsDead()
{
	if (getHealth() <= 0)
	{
		setHealth(0);

		return true;
	}
	return false;
}
glm::vec3 Player::getPos() const
{
	return modelMatrix[3];
}
std::string Player::type() const
{
	return "Player";
}

//Update function
void Player::update(sf::Window &window, float dt, std::vector<Model*> &allModels, std::vector<Enemy*> allEnemies)
{
	allAttackBoxes.clear();
	for (int i = 0; i < allArrowAttackBoxes.size(); i++)
	{
		allAttackBoxes.push_back(allArrowAttackBoxes[i]);
	}
	for (int i = 0; i < allAOEAttackBoxes.size(); i++)
	{
		allAttackBoxes.push_back(allAOEAttackBoxes[i]);
	}
	for (int i = 0; i < allMeleeAttackBoxes.size(); i++)
	{
		allAttackBoxes.push_back(allMeleeAttackBoxes[i]);
	}

	allStaticModels = allModels;

	//Kill player and reset jumps in water
	if (getDiving())
	{
		jumps = 0;
		waterEffect();
	}
	
	//groundCheck();

	if (getPos().y > groundPos && isOnGround)
	{
		isOnGround = false;
	}

	int controller = CONTROLLER0;

	//Move
	if(this->diving == false)
	{
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
			velocityY -= 30 * dt;
		}

		//Maximum falling speed
		if (velocityY < -30)
		{
			velocityY = -30;
		}
	}
	else 
	{
		if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) < -20)
		{
			velocityX = -movementSpeed*dt/2;
		}
		else if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) > 20)
		{
			velocityX = movementSpeed*dt/2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			velocityX = -movementSpeed*dt/2;
			goingLeft = true;
			goingRight = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			velocityX = movementSpeed*dt/2;
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
				this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				angle -= 12;
			}
		}
		//If in air
		if (!isOnGround)
		{
			velocityY -= 30 * dt * 2;
		}

		//Maximum falling speed
		if (velocityY < -5)
		{
			velocityY = -5;
		}
	}

	if (playerCharacters[2] == player)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && tpCooldown.getElapsedTime().asSeconds() >= 5.0)
		{
			glm::vec3 prevPos = getPos();
			if (goingLeft == true)
			{
				glm::vec3 minus4 = {-4,0,0};
				this->setPos(this->getPos() + minus4);
			}
			else
			{
				glm::vec3 plus4 = {4,0,0};
				this->setPos(this->getPos() + plus4);
			}

			//colisions
			bool colided = false;
			for(int index = 0; index < allModels.size() && colided == false; index++)
			{
				std::vector<glm::vec2> playerPoints = getPoints();
				std::vector<glm::vec2> objectPoints;
				float radians = 0.0f;
				getPoints(objectPoints, allModels[index], radians);
				glm::vec2 mtv;
				colided = collision::collision(playerPoints, objectPoints, mtv);
				if (colided == false)
				{
					colided = collision::isInside(playerPoints, objectPoints);
				}
				
			}
			if (colided == true)
			{
				this->setPos(prevPos);
			}

			tpCooldown.restart();
		}
	}

	//Apply velocity
	modelMatrix[3].x += velocityX;
	velocityX = 0;
	modelMatrix[3].y += velocityY*dt;
	
	//collision(allModels);

	//Handle collision detection with ground
	if (getPos().y <= groundPos && !isOnGround)
	{
		jumps = 0;
		if (velocityY < 0)
		{
			modelMatrix[3].y = groundPos;
			velocityY = 0;
		}
		isOnGround = true;
	}

	for (int i = 0; i < allAttackBoxes.size(); i++)
	{
		if (allAttackBoxes[i]->isInUse())
		{
			if (glm::distance(getPos(), allAttackBoxes[i]->getPos()) < 40.0f)
			{
				allAttackBoxes[i]->update(dt, allModels, getPos());
				std::vector<glm::vec2> arrowPoints = allAttackBoxes[i]->getPoints();
				for (int k = 0; k < allEnemies.size(); k++)
				{
					if (!allAttackBoxes[i]->isCollidingWithWorld() && glm::distance(allAttackBoxes[i]->getPos(), allEnemies[k]->getPos()) < 2.0f)
					{
						if (collision::collision(arrowPoints, allEnemies[k]->getPoints()))
						{
							if (allAttackBoxes[i]->isProjectileAttack())
							{
								allAttackBoxes[i]->disableArrow();
							}
							allEnemies[k]->applyDamage(100);
							k = (int)allEnemies.size();
						}
					}
				}
			}
			else
			{
				allAttackBoxes[i]->disableArrow();
			}
		}
	}
	//Check if aiming
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		aiming(window, dt);
	}
}

//Draws the models involved
void Player::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
	player->draw(shader);

	for (int i = 0; i < allAttackBoxes.size(); i++)
	{
		if (allAttackBoxes[i]->isInUse())
		{
			allAttackBoxes[i]->draw(shader);
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && player == playerCharacters[1])
	{
		for (int i = 0; i < debugCubes.size(); i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &debugCubes[i]->getModelMatrix()[0][0]);
			debugCubes[i]->draw(shader);
		}
	}
}

//Tests collision with other objects
void Player::collision(std::vector<Model*> &allModels)
{
	bool collides = true;
	int collisionChecks = 0;
	while (collides && collisionChecks < 5)
	{
		std::vector<Model*> closeObjects;
		for (int i = 0; i < allModels.size(); i++)
		{
			glm::vec3 objectMin, objectMax;
			allModels[i]->getScaledMinMaxBouding(objectMin, objectMax);
			glm::vec2 distance = allModels[i]->getPos() - getPos();
			if (abs(distance.x) < 2.0f+objectMax.x)
			{
				if (abs(distance.y) < 2.0f + objectMax.y)
				{
					closeObjects.push_back(allModels[i]);
				}
			}
		}

		if (closeObjects.size())
		{
			std::vector<glm::vec2> playerPoints = getPoints();

			for (int i = 0; i < closeObjects.size(); i++)
			{
				std::vector<glm::vec2> objectPoints;
				float radians = 0.0f;
				getPoints(objectPoints, closeObjects[i], radians);
				glm::vec2 mtv;
				if (collision::collision(playerPoints, objectPoints, mtv))
				{
					if (radians > 0.0f && radians < 0.79f)
					{
						if (mtv.y > 0)
						{
							modelMatrix[3].y -= 0.10f;
						}
						else
						{
							velocityY -= 0.5f;
						}
					}
					else
					{
						modelMatrix[3].x += mtv.x;
						if (mtv.y < 0)
						{
							velocityY = 0;
						}
					}
					modelMatrix[3].y += mtv.y;
					if (mtv.y > 0)
					{
						if (modelMatrix[3].y < 0) modelMatrix[3].y = 0;
						groundPos = modelMatrix[3].y;
					}
				}
				else
				{
					collides = false;
				}
			}
		}
		else
		{
			collides = false;
		}
		collisionChecks++;
		/*
		int index = -1;
		float minDistance = 1000;
		glm::vec2 player2dPos = glm::vec2(getPos().x, getPos().y + 0.5f);
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
			std::vector<glm::vec2> playerPoints = getPoints();
			std::vector<glm::vec2> objectPoints;
			float radians = 0.0f;
			getPoints(objectPoints, allModels[index], radians);
			glm::vec2 mtv;
			if (collision::collision(playerPoints, objectPoints, mtv))
			{
				if (radians > 0.0f && radians < 0.79f)
				{
					if (mtv.y > 0)
					{
						modelMatrix[3].y -= 0.05f;
					}
					else
					{
						velocityY -= 0.5f;
					}
				}
				else
				{
					modelMatrix[3].x += mtv.x;
					if (mtv.y < 0)
					{
						velocityY = 0;
					}
				}
				modelMatrix[3].y += mtv.y;
				if (mtv.y > 0)
				{
					if (modelMatrix[3].y < 0) modelMatrix[3].y = 0;
					groundPos = modelMatrix[3].y;
				}
			}
			else
			{
				collides = false;
			}
		}
		collisionChecks++;
		*/
	}
}
void Player::getPoints(std::vector<glm::vec2> &objectPoints, Model *object, float &radians)
{
	//Get rotation and scale from modelMat
	glm::mat4 modelMat = object->getModelMatrix();
	glm::vec3 scale;
	glm::quat rotation;
	glm::decompose(modelMat, scale, rotation, glm::vec3(), glm::vec3(), glm::vec4());

	//Convert from quat to radians
	double t3 = +2.0 * (rotation.w * rotation.z + rotation.x * rotation.y);
	double t4 = +1.0 - 2.0f * ((rotation.y * rotation.y) + rotation.z * rotation.z);
	radians = (float)-std::atan2(t3, t4);

	//Get object points
	objectPoints = object->getPoints();
}
void Player::setStaticModels(std::vector<Model*> theModels)
{
	this->allStaticModels = theModels;
}

bool Player::getDiving() const
{
	return this->diving;
}

void Player::setDiving(bool diving)
{
	this->diving = diving;
}

void Player::setHealth(int health)
{
	this->health = health;
}

void Player::groundCheck()
{
	//Sort models by x axis
	std::vector<Model*> sortedModels;
	for (int i = 0; i < allStaticModels.size(); i++)
	{
		glm::vec3 min, max;
		allStaticModels[i]->getMinMaxBouding(min, max);
		min = min * pow(5.f,2);
		max = max * pow(5.f,2);
		min += allStaticModels[i]->getPos();
		max += allStaticModels[i]->getPos();
		if (getPos().x >= min.x && getPos().x <= max.x)
		{
			sortedModels.push_back(allStaticModels[i]);
		}
	}
	//Now find the ground based on the sorted models
	bool foundGround = false;
	float closestDistance = 100;
	glm::vec3 rayOrigin = glm::vec3(getPos().x, getPos().y + 2.0f, getPos().z);
	glm::vec3 rayDir(0, -1, 0);
	for (int i = 0; i < sortedModels.size(); i++)
	{
		glm::vec3 rayOrigin = glm::vec3(getPos().x, getPos().y + 2.0f, getPos().z);
		glm::vec3 rayDir(0, -1, 0);
		glm::vec3 aabbMin, aabbMax;
		sortedModels[i]->getMinMaxBouding(aabbMin, aabbMax);
		aabbMin = aabbMin * pow(5.f, 2);
		aabbMax = aabbMax * pow(5.f, 2);
		glm::mat4 boxMat = sortedModels[i]->getModelMatrix();
		float distance = 10000;
		
		//Raycast downwards to find the distance to ground
		if (collision::TestRayOBBIntersection(rayOrigin, rayDir, aabbMin, aabbMax, boxMat, distance))
		{
			if (distance < closestDistance)
			{
				closestDistance = distance;
				foundGround = true;
			}
		}
	}
	if (foundGround)
	{
		groundPos = rayOrigin.y - closestDistance;
	}
	else
	{
		groundPos = 0;
	}
}
