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

	this->playerPos = glm::vec3(0.0f, 2.0f, 0.0f);
	setActualPos(playerPos);
	this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->modelMatrix *= glm::scale(glm::vec3(0.08f, 0.08f, 0.08f));
	angle = 0;
	this->movementSpeed = 4.0f;
	//Add characters
	this->playerCharacters[0] = new PlayerBird(10000, birdModel);
	this->playerCharacters[1] = new PlayerShark(10000, sharkModel);
	this->playerCharacters[2] = new PlayerButterfly(10000, butterflyModel);
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

glm::vec3 Player::getPlayerPos() const
{
	return this->playerPos;
}

glm::vec3 Player::getActualPlayerPos() const
{
	return this->modelMatrix[3];
}
std::vector<glm::vec2> Player::getPoints()
{
	return getPlayerPoints();
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
		velocityY = player->getJumpHeight();
		jumps++;
	}
}
//Makes the player shoot
void Player::shoot(sf::Window &window)
{
	int activeArrows = 0;
	for (int i = 0; i < arrows.size(); i++)
	{
		if (arrows[i]->isInUse())
			activeArrows++;

	}
	if (activeArrows < arrows.size())
	{
		for (int i = 0; i < arrows.size(); i++)
		{
			if (!arrows[i]->isInUse())
			{
				arrows[i]->shoot(window, glm::vec2(playerPos.x, playerPos.y + 2.f), arrow);
				i = (int)arrows.size();
			}
		}
	}
	else
	{
		Projectile* temp = new Projectile();
		temp->shoot(window, glm::vec2(playerPos.x, playerPos.y + 2.f), arrow);
		arrows.push_back(temp);
	}
}
//Sets the playerPos variable
void Player::setPos(glm::vec3 playerPos)
{
	this->playerPos = playerPos;
}
//Sets the actual model position in the model matrix
void Player::setActualPos(glm::vec3 playerPos)
{
	this->modelMatrix[3] = glm::vec4(playerPos,1.0f);
}
//True if player is dead
bool Player::playerIsDead()
{
	if (playerCharacters[0]->getHP() <= 0)
	{
		playerCharacters[0]->setHP(0);

		return true;
	}
	return false;
}
glm::vec3 Player::getPos() const
{
	return getActualPlayerPos();
}
std::string Player::type() const
{
	return "Player";
}
//Update function
void Player::update(sf::Window &window, float dt, std::vector<Model*> &allModels, glm::vec3 enemyPos, int enemyDamage)
{
	groundPos = 0.0f;

	/*if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (arrow == nullptr)
		{
			glm::mat4 modelMat({
				1.0, 0.0, 0.0, 0.0,
				0.0, 0.2, 0.0, 0.0,
				0.0, 0.0, 0.2, 0.0,
				playerPos.x, playerPos.y+1.5f, 0.0, 1.0
			});
			arrow = new Model(playerCharacters[2]->getModel(), modelMat);
		}
		else
		{
			arrow->setModelMatrix({
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				playerPos.x, playerPos.y+1.5f, 0.0, 1.0
			});
			glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
			glm::vec2 middleScreen(window.getSize().x/2, window.getSize().y/2);
			arrowRotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
			arrow->setRotationMatrix(glm::rotate(glm::mat4(), arrowRotation, glm::vec3(0.0f, 0.0f, 1.0f)));
			arrow->rotate();
			arrowDirection = glm::normalize(glm::vec2(sin(arrowRotation), -cos(arrowRotation)));
			arrowVelocity = glm::vec2(glm::abs(arrowDirection.x),arrowDirection.y) * 0.5f;
			std::cout << arrowVelocity.y << std::endl;
		}
	}
	if (arrow != nullptr)
	{
		glm::vec2 arrowPos(arrow->getModelMatrix()[3]);
		arrow->setModelMatrix({
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			arrowPos.x + (arrowVelocity.x*arrowDirection.x), arrowPos.y + arrowVelocity.y , 0.0, 1.0
		});
		arrow->setRotationMatrix(glm::rotate(glm::mat4(), arrowRotation, glm::vec3(0.0f, 0.0f, 1.0f)));
		arrow->rotate();
		arrowVelocity.x -= 0.1*dt;
		if (arrowVelocity.x < 0) arrowVelocity.x = 0;
		arrowVelocity.y -= 0.5*dt;
		std::vector<glm::vec2> arrowPoints = arrow->getPoints(glm::vec3(1.f));
		for (int i = 0; i < allModels.size(); i++)
		{
			std::vector<glm::vec2> objectPoints = allModels[i]->getPoints(glm::vec3(5.f));
			glm::vec2 mtv;
			if (collision::fixCollision(arrowPoints, objectPoints, mtv))
			{
				arrowVelocity = glm::vec2(0);
			}
		}
	}*/

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
		float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
		glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));

		glm::vec2 startposition = glm::vec2(playerPos.x, playerPos.y + 2.f);
		glm::vec2 startvelocity = glm::vec2(glm::abs(direction.x*0.5f), direction.y*0.5f);

		for (int i = 0; i < 30; i++)
		{
			glm::vec2 velocity;
			glm::vec2 position = startposition;

			velocity.x = startvelocity.x - 0.1f*(float)i;
			velocity.y = startvelocity.y - 0.5f*(float)i;
			position.x += direction.x*velocity.x;
			position.y += velocity.y;

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
	for (int i = 0; i < arrows.size(); i++)
	{
		if (arrows[i]->isInUse())
		{
			arrows[i]->update(dt, allModels);
		}
	}

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
	
	testCollision(allModels);

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
	setActualPos(playerPos);
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
void Player::testCollision(std::vector<Model*> &allModels)
{
	bool collides = true;
	int collisionChecks = 0;
	while (collides && collisionChecks < 5)
	{
		int index = -1;
		float minDistance = 1000;
		glm::vec2 player2dPos = glm::vec2(playerPos.x, playerPos.y + 0.5f);
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
			std::vector<glm::vec2> playerPoints = getPlayerPoints();
			std::vector<glm::vec2> objectPoints;
			float radians = 0.0f;
			getPoints(objectPoints, allModels[index], radians);
			glm::vec2 mtv;
			if (collision::testCollision(playerPoints, objectPoints, mtv))
			{
				if (radians > 0.0f && radians < 0.79f)
				{
					if (mtv.y > 0)
					{
						playerPos.y -= 0.05f;
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
			else
			{
				collides = false;
			}
		}
		collisionChecks++;
	}
}
//Get playerPoints
std::vector<glm::vec2> Player::getPlayerPoints()
{
	std::vector<glm::vec2> playerPoints;
	playerPoints.push_back(glm::vec2(-0.5f, -0.0f));
	playerPoints.push_back(glm::vec2(0.5f, -0.0f));
	playerPoints.push_back(glm::vec2(0.5f, 1.0f));
	playerPoints.push_back(glm::vec2(-0.5f, 1.0f));
	for (int k = 0; k < playerPoints.size(); k++)
	{
		playerPoints[k] += glm::vec2(playerPos);
	}
	return playerPoints;
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