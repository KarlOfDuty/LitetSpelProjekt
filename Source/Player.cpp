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


	Model* birdModel = new Model();
	birdModel->readModel("models/Characters/Bird/model.bb");
	birdModel->loadWeight("models/Characters/Bird/weightInfo.bb");
	birdModel->loadSkeleton("models/Characters/Bird/Idle/skelInfo.bb");
	birdModel->loadSkeleton("models/Characters/Bird/Run/skelInfo.bb");
	birdModel->loadSkeleton("models/Characters/Bird/Actions/skelInfo.bb");
	birdModel->loadSkeleton("models/Characters/Bird/Jump/RunningJump/skelInfo.bb");
	birdModel->loadSkeleton("models/Characters/Bird/Jump/StillJump/skelInfo.bb");
	birdModel->loadSkeleton("models/Characters/Bird/Special/skelInfo.bb");
	birdModel->setupModel();

	Model* sharkModel = new Model();
	sharkModel->readModel("models/Characters/Shark/model.bb");
	sharkModel->loadWeight("models/Characters/Shark/weightInfo.bb");
	sharkModel->loadSkeleton("models/Characters/Shark/Idle/skelInfo.bb");
	//sharkModel->loadSkeleton("models/Characters/Shark/Run/skelInfo.bb");
	//sharkModel->loadSkeleton("models/Characters/Shark/Jump/StillJump/skelInfo.bb");
	//sharkModel->loadSkeleton("models/Characters/Shark/Special/skelInfo.bb");
	sharkModel->setupModel();

	Model* butterflyModel = new Model();
	butterflyModel->readModel("models/Characters/Butter/model.bb");
	butterflyModel->loadWeight("models/Characters/Butter/weightInfo.bb");
	butterflyModel->loadSkeleton("models/Characters/Butter/Idle/skelInfo.bb");
	butterflyModel->loadSkeleton("models/Characters/Butter/Run/skelInfo.bb");
	butterflyModel->loadSkeleton("models/Characters/Butter/Actions/skelInfo.bb");
	butterflyModel->loadSkeleton("models/Characters/Butter/Jump/StillJump/skelInfo.bb");
	butterflyModel->loadSkeleton("models/Characters/Butter/Jump/RunningJump/skelInfo.bb");
	butterflyModel->loadSkeleton("models/Characters/Butter/Special/skelInfo.bb");
	butterflyModel->setupModel();
	//Model* sharkModel = new Model("models/Characters/Shark/Fish_T-Pose_Export.obj", modelMatrix);
	//Model* butterflyModel = new Model("models/Characters/Butterfly/ButterFly.obj", modelMatrix);

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
	this->goingRight = true;

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
	playerPoints.push_back(glm::vec2(-10.0f, 0.0f));
	playerPoints.push_back(glm::vec2(10.0f, 35.0f));
	playerPoints.push_back(glm::vec2(10.0f, 0.0f));
	playerPoints.push_back(glm::vec2(-10.0f, 35.0f));
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
	if (player->getMaxJumps() > jumps)
	{
		if (diving)
		{
			velocityY = player->getJumpHeight();			
		}
		else
		{
			velocityY = player->getJumpHeight();
			jumps++;
		}
	}
	if (jumps == 2) {
		//player->getModel()->setCurrentKeyframe(1);
		player->getModel()->setAnimationIndex(6);
	}
}


void Player::waterEffect()
{
	if (this->player != playerCharacters[1])
	{
		this->applyDamage(2);
	}
}

void Player::applyDamage(int appliedDamage)
{
	if (this->damageImmunity.getElapsedTime().asSeconds() >= 1.5f)
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
		glm::vec2 position = (mouseX >= middleScreenX) ? glm::vec2(getPos().x + 50.0f, getPos().y) : glm::vec2(getPos().x - 50.0f, getPos().y);
		butterfly->shootAoe(allStaticModels, allAOEAttackBoxes, position);
	}
	PlayerBird* bird = dynamic_cast<PlayerBird*>(player);
	if (bird != nullptr)
	{
		int mouseX = sf::Mouse::getPosition(window).x;
		int middleScreenX = window.getSize().x / 2;
		glm::vec2 position;
		glm::vec2 direction;
		if (goingRight)
		{
			position = glm::vec2(getPos().x + 10.0f, getPos().y);
			direction = glm::vec2(1, 0);
		}
		else
		{
			position = glm::vec2(getPos().x - 10.0f, getPos().y);
			direction = glm::vec2(-1, 0);
		}
		bird->meleeAttack(allMeleeAttackBoxes, position, direction, 150.f);
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
		glm::vec2 startPos = glm::vec2(getPos().x, getPos().y + 30.0f);
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
		glm::vec2 position = (mouseX >= middleScreenX) ? glm::vec2(getPos().x + 150.0f, getPos().y) : glm::vec2(getPos().x - 150.0f, getPos().y);
		butterfly->shootAoe(allStaticModels, allAOEAttackBoxes, position);
	}
	PlayerBird* bird = dynamic_cast<PlayerBird*>(player);
	if (bird != nullptr)
	{
		glm::vec2 position;
		glm::vec2 direction;
		if (goingRight)
		{
			position = glm::vec2(getPos().x + 10.0f, getPos().y);
			direction = glm::vec2(1, 0);
		}
		else
		{
			position = glm::vec2(getPos().x - 10.0f, getPos().y);
			direction = glm::vec2(-1, 0);
		}
		bird->meleeAttack(allMeleeAttackBoxes, position, direction, 10.0f);
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
		glm::vec2 startPos = glm::vec2(getPos().x, getPos().y + 30.0f);
		bird->shootArrow(allArrowAttackBoxes, startPos, direction);
		bird->arrowVelocity = 300.0f;
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
		if (bird->arrowVelocity >= 600.0f)
		{
			bird->arrowVelocity = 600.0f;
		}
		else
		{
			bird->arrowVelocity += 200.0f * dt;
		}

		glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
		float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
		glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));

		glm::vec2 position = glm::vec2(getPos().x, getPos().y + 30.f);
		glm::vec2 velocity = glm::vec2(glm::abs(direction.x*bird->arrowVelocity), direction.y*bird->arrowVelocity);
		for (int i = 0; i < 30; i++)
		{
			velocity.x -= 50.0f*0.02f;
			if (velocity.x < 0) velocity.x = 0;
			velocity.y -= 300.0f*0.02f;
			position.x += direction.x*velocity.x*0.02f;
			position.y += velocity.y*0.02f;

			glm::mat4 modelMat({
				2.0, 0.0, 0.0, 0.0,
				0.0, 2.0, 0.0, 0.0,
				0.0, 0.0, 2.0, 0.0,
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		std::cout << this->getPos().x << " x" << std::endl;
		std::cout << this->getPos().y << " y" << std::endl;
	}

	player->update(dt);
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
	
	groundCheck();

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
			if (getPos().y < (groundPos + 15.0f))
			{
				if (timeSinceJump.getElapsedTime().asSeconds() > 0.2)
				{
					if (timeSinceAttack.getElapsedTime().asSeconds() > 2.0)
					{
						setAnimationIndex(1);
					}
				}
				isIdle = false;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			velocityX = movementSpeed*dt;
			goingRight = true;
			goingLeft = false;
			if (getPos().y < (groundPos + 15.0f))
			{
				if (timeSinceJump.getElapsedTime().asSeconds() > 0.2)
				{
					if (timeSinceAttack.getElapsedTime().asSeconds() > 2.0)
					{
						setAnimationIndex(1);
					}
				}
				isIdle = false;
			}
		}

		if (goingLeft == true)
		{
			if (angle != 180)
			{
				this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				angle += 20;
				rotating.restart();
			}
		
		}
		if (goingRight == true)
		{
			if (angle > 0)
			{
				this->modelMatrix  *= glm::rotate(glm::mat4(), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				angle -= 20;
				rotating.restart();
			}
		}
		//If in air
		if (!isOnGround)
		{
			velocityY -= 305 * dt;
		}

		//Maximum falling speed
		if (velocityY < -900)
		{
			velocityY = -900;
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
			if (getPos().y < (groundPos + 15.0f))
			{
				isIdle = false;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			velocityX = movementSpeed*dt/2;
			goingRight = true;
			goingLeft = false;
			if (getPos().y < (groundPos + 15.0f))
			{
				isIdle = false;
			}
		}

		if (goingLeft == true)
		{
			if (angle != 180)
			{
				this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				angle += 20;
				rotating.restart();
			}
		}

		if (goingRight == true)
		{
			if (angle > 0)
			{
				this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				angle -= 20;
				rotating.restart();
			}
		}
		//If in air
		if (!isOnGround)
		{
			velocityY -= 305 * dt * 2;
		}

		//Maximum falling speed
		if (velocityY < -150)
		{
			velocityY = -150;
		}
	}
	
	if (playerCharacters[2] == player)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && tpCooldown.getElapsedTime().asSeconds() >= 3.0)
		{
			glm::vec3 prevPos = getPos();
			if (goingLeft == true)
			{
				glm::vec3 minus4 = {-50,00,0};
				this->setPos(this->getPos() + minus4);
			}
			else
			{
				glm::vec3 plus4 = {50,00,0};
				this->setPos(this->getPos() + plus4);
			}

			//colisions
			bool colided = false;
			glm::vec2 mtvValue(0, 0);
			for(int index = 0; index < allModels.size() && colided == false; index++)
			{
				std::vector<glm::vec2> playerPoints = getPoints();
				std::vector<glm::vec2> objectPoints;
				float radians = 0.0f;
				getPoints(objectPoints, allModels[index], radians);
				glm::vec2 mtv;
				collision::collision(playerPoints, objectPoints, mtv);
				if (mtv != glm::vec2(0,0) && abs(mtv.x) < 20 && abs(mtv.y) < 20)
				{
					mtvValue = mtv;
				}
				if (mtv == glm::vec2(0,0))
				{
					colided = collision::isInside(playerPoints, objectPoints);
				}
				
			}
			if (colided == true)
			{
				this->setPos(prevPos);
			}
			else
			{
				tpCooldown.restart();
			}
		}
	}

	if (velocityX == 0)
	{
		if (getPos().y < (groundPos + 15.0f))
		{
			//Change the time compare to make it idle faster after turning/rotating
			if (rotating.getElapsedTime().asSeconds() >= 0.2)
			{
				isIdle = true;
			}
		}
	}

	if (getPos().y > (groundPos + 10.0f))
	{
		if (timeSinceAttack.getElapsedTime().asSeconds() > 1.0)
		{
			setAnimationIndex(4);
			timeSinceJump.restart();
		}
	}

	if (timeSinceJump.getElapsedTime().asSeconds() > 1.1)
	{
		if (timeSinceAttack.getElapsedTime().asSeconds() > 1.0)
		{
			if (isIdle && getPos().y < (groundPos + 15.0f))
			{
				setAnimationIndex(0);
				this->player->getModel()->resetKeyframe();
			}
		}
	}

	if (timeSinceAttack.getElapsedTime().asSeconds() > 1.0)
	{
		attacking = false;
	}

	if (attacking)
	{
		if (timeSinceAttack.getElapsedTime().asSeconds() < 1.0)
		{
			setAnimationIndex(2);
			//this->player->getModel()->resetKeyframe();
		}
	}

	//Apply velocity
	modelMatrix[3].x += velocityX;
	velocityX = 0;
	modelMatrix[3].y += velocityY*dt;
	
	collision(allModels);

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
			if (glm::distance(getPos(), allAttackBoxes[i]->getPos()) < 500.0f)
			{
				allAttackBoxes[i]->update(dt, allModels, getPos(), goingRight);
				std::vector<glm::vec2> arrowPoints = allAttackBoxes[i]->getPoints();
				for (int k = 0; k < allEnemies.size(); k++)
				{
					if (!allAttackBoxes[i]->isCollidingWithWorld() && glm::distance(allAttackBoxes[i]->getPos(), allEnemies[k]->getPos()) < 200.0f)
					{
						if (collision::collision(arrowPoints, allEnemies[k]->getPoints()))
						{
							if (allAttackBoxes[i]->isProjectileAttack())
							{
								allAttackBoxes[i]->disableArrow();
							}
							allEnemies[k]->applyDamage(allAttackBoxes[i]->getDamage());
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

	for (int i = 0; i < allArrowAttackBoxes.size(); i++)
	{
		if (allArrowAttackBoxes[i]->isInUse())
		{
			allArrowAttackBoxes[i]->draw(shader);
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
			if (abs(distance.x) < 50.0f+objectMax.x)
			{
				if (abs(distance.y) < 50.0f + objectMax.y)
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
				glm::vec2 collisionNormal;
				if (collision::collision(playerPoints, objectPoints, mtv, collisionNormal))
				{
					if (collisionNormal.y >= 0.707)
					{
						modelMatrix[3].y += mtv.y;
						
						if (modelMatrix[3].y < groundPos)
						{
							modelMatrix[3].y = groundPos;
						}
						if (velocityY < 0)
						{
							velocityY = 0;
						}
						groundPos = modelMatrix[3].y;
						jumps = 0;
						isOnGround = true;
					}
					if (collisionNormal.y < 0 && velocityY > 0)
					{
						modelMatrix[3].y += mtv.y;
						velocityY = velocityY * (collisionNormal.y + 1.0);
					}
					if (abs(collisionNormal.x) >= 0.707)
					{
						modelMatrix[3].x += mtv.x;
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

void Player::setCurrentKeyframe(int frame)
{
	this->player->getModel()->setCurrentKeyframe(frame);
}

void Player::setAnimationIndex(int index)
{
	this->player->getModel()->setAnimationIndex(index);
}

void Player::restartTimeSinceJump()
{
	this->timeSinceJump.restart();
}

void Player::restartTimeSinceAttack()
{
	timeSinceAttack.restart();
}

void Player::setAttacking(bool attacking)
{
	this->attacking = attacking;
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
	if (health > 20)
	{
		health = 20;
	}
	this->health = health;
}

void Player::groundCheck()
{
	//Sort models by x axis
	std::vector<Model*> sortedModels;
	for (int i = 0; i < allStaticModels.size(); i++)
	{
		glm::vec3 min, max;
		allStaticModels[i]->getScaledMinMaxBouding(min, max);
		min += allStaticModels[i]->getPos();
		max += allStaticModels[i]->getPos();
		if (getPos().x >= min.x && getPos().x <= max.x)
		{
			sortedModels.push_back(allStaticModels[i]);
		}
	}
	//Now find the ground based on the sorted models
	bool foundGround = false;
	float closestDistance = 100000;
	glm::vec3 rayOrigin = glm::vec3(getPos().x - 9, getPos().y, getPos().z);
	glm::vec3 rayDir(0, -1, 0);
	for (int i = 0; i < sortedModels.size(); i++)
	{
		glm::vec3 aabbMin, aabbMax;
		glm::vec3 scale;
		glm::decompose(sortedModels[i]->getModelMatrix(), scale, glm::quat(), glm::vec3(), glm::vec3(), glm::vec4());
		sortedModels[i]->getMinMaxBouding(aabbMin, aabbMax);
		aabbMin = aabbMin * scale * scale;
		aabbMax = aabbMax * scale * scale;
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
	rayOrigin = glm::vec3(getPos().x + 9, getPos().y, getPos().z);
	rayDir = glm::vec3(0, -1, 0);
	for (int i = 0; i < sortedModels.size(); i++)
	{
		glm::vec3 aabbMin, aabbMax;
		glm::vec3 scale;
		glm::decompose(sortedModels[i]->getModelMatrix(), scale, glm::quat(), glm::vec3(), glm::vec3(), glm::vec4());
		sortedModels[i]->getMinMaxBouding(aabbMin, aabbMax);
		aabbMin = aabbMin * scale * scale;
		aabbMax = aabbMax * scale * scale;
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
