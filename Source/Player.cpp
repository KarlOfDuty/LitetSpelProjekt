#include "Player.h"

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
	birdModel = Model("models/Characters/Bird/BirdTest1.obj", modelMatrix);
	sharkModel = Model("models/sphere/sphere.obj", modelMatrix);
	butterflyModel = Model("models/cube/cubeGreen.obj", modelMatrix);

	this->playerPos = glm::vec3(0.0f, 0.0f, 1.0f);
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
  
//Update funtion
void Player::update(float dt, sf::Window &window)
{
	if (playerPos.y > 0.0f)
	{
		isOnGround = false;
	}
	int controller = CONTROLLER0;
	//Move
	if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) < -20)
	{
		playerPos.x -= movementSpeed*dt;
	}
	else if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) > 20)
	{
		playerPos.x += movementSpeed*dt;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		playerPos.x -= movementSpeed*dt;
		goingLeft = true;
		goingRight = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		playerPos.x += movementSpeed*dt;
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
		velocityY -= 0.5*dt;
	}

	//Maximum falling speed
	if (velocityY > 5)
	{
		velocityY = 5;
	}

	//Apply velocity
	playerPos.y += velocityY;

	//Handle collision detection with ground
	if (playerPos.y <= 0)
	{
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
	player->draw(shader);
}
