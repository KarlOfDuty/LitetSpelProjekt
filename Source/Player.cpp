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

	arrow = new Model("models/cube/cubeGreen.obj");

	this->modelMatrix[3] = glm::vec4(0.0f, 2.0f, 0.0f, 1.0);
	this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->modelMatrix *= glm::scale(glm::vec3(0.08f, 0.08f, 0.08f));

	angle = 0;
	this->movementSpeed = 4.0f;
	//Add characters
	this->playerCharacters[0] = new PlayerBird(10000, birdModel, false);
	this->playerCharacters[1] = new PlayerShark(10000, sharkModel, false);
	this->playerCharacters[2] = new PlayerButterfly(10000, butterflyModel, false);
	this->player = playerCharacters[0];
	this->isOnGround = true;

	arrows = std::vector<Projectile*>();
}

Player::~Player()
{
	this->freeMemory();
}

PlayerChar* Player::getCurrentCharacter()
{
	return player;
}

int Player::getDamage() const
{
	return playerCharacters[0]->getDamage();
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
	if(player->getDiving() != true)
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
//Makes the player shoot
void Player::shoot(sf::Window &window)
{
	//Check how many arrows are active in the arrow vector
	int activeArrows = 0;
	for (int i = 0; i < arrows.size(); i++)
	{
		if (arrows[i]->isInUse())
			activeArrows++;
	}
	
	//Get direction and scale
	glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
	float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
	glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));
	glm::vec3 scale(0.1f, 1.0f, 0.1f);

	//Reuse old arrow if possible otherwize create a new
	if (activeArrows < arrows.size())
	{
		for (int i = 0; i < arrows.size(); i++)
		{
			if (!arrows[i]->isInUse())
			{
				arrows[i]->shoot(arrow, glm::vec2(getPos().x, getPos().y + 2.0f), direction, glm::vec2(5.0f, 30.0f), arrowVelocity, scale, true);
				i = (int)arrows.size();
			}
		}
	}
	else
	{
		Projectile* temp = new Projectile();
		temp->shoot(arrow, glm::vec2(getPos().x, getPos().y + 2.0f), direction, glm::vec2(5.0f,30.0f) , arrowVelocity, scale, true);
		arrows.push_back(temp);
	}
}

void Player::aiming(sf::Window &window,float dt)
{
	glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
	float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
	glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));

	glm::vec2 position = glm::vec2(getPos().x, getPos().y + 2.f);
	glm::vec2 velocity = glm::vec2(glm::abs(direction.x*arrowVelocity), direction.y*arrowVelocity);
	for (int i = 0; i < 30; i++)
	{
		velocity.x -= 5.0f*0.01;
		if (velocity.x < 0) velocity.x = 0;
		velocity.y -= 40.0f*0.01;
		position.x += direction.x*velocity.x*0.02;
		position.y += velocity.y*0.02;

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

//Sets the playerPos variable
void Player::setPos(glm::vec3 playerPos)
{
	this->modelMatrix[3] = glm::vec4(playerPos,1.0);
}
//True if player is dead
bool Player::playerIsDead()
{
	if (playerCharacters[0]->getHealth() <= 0)
	{
		playerCharacters[0]->setHealth(0);

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
	groundPos = 0.0f;

	for (int i = 0; i < arrows.size(); i++)
	{
		if (arrows[i]->isInUse())
		{
			if (glm::distance(getPos(), arrows[i]->getPos()) < 40.0f)
			{
				arrows[i]->update(dt, allModels);
				std::vector<glm::vec2> arrowPoints = arrows[i]->getPoints();
				for (int k = 0; k < allEnemies.size(); k++)
				{
					if (glm::distance(arrows[i]->getPos(), allEnemies[k]->getPos()) < 2.0f)
					{
						if (collision::collision(arrowPoints, allEnemies[k]->getPoints()))
						{
							arrows[i]->disableArrow();
							allEnemies[k]->applyDamage(100);
							k = allEnemies.size();
						}
					}
				}
			}
			else
			{
				arrows[i]->disableArrow();
			}
		}
	}
	
	//Check if aiming
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		aiming(window,dt);
		if (arrowVelocity >= 60.0f)
		{
			arrowVelocity = 60.0f;
		}
		else
		{
			arrowVelocity += 5.0f * dt;
		}
	}
	else
	{
		arrowVelocity = 30.0f;
	}

	if (getPos().y > groundPos && isOnGround)
	{
		isOnGround = false;
	}
	int controller = CONTROLLER0;
	//Move
	if(player->getDiving() != true)
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
			tpCooldown.restart();
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
}
//Draws the models involved
void Player::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
	player->draw(shader);

	for (int i = 0; i < arrows.size(); i++)
	{
		if (arrows[i]->isInUse())
		{
			arrows[i]->draw(shader);
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
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