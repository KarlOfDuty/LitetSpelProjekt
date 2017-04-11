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
	birdModel = Model("models/cube/cube.obj");
	sharkModel = Model("models/cube/cube.obj");
	butterflyModel = Model("models/cube/cube.obj");

	this->playerPos = glm::vec3(0.0f, 0.0f, 0.0f);

	setPos(playerPos);

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

void Player::jump(float dt)
{
	if (player->getMaxJumps() > jumps)
	{
		velocityY = 10 * dt;
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
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		playerPos.x += movementSpeed*dt;
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
	playerCharacters[0]->draw(shader);
}
