#include "EnemyChar.h"


EnemyChar::EnemyChar()
{
	this->HP = 50;
	this->damage = 10;
}

EnemyChar::EnemyChar(int HP, Model model, int damage, glm::vec3 enemyStartPos)
{
	this->HP = HP;
	this->enemyModel = model;
	this->damage = damage;
	this->enemyPos = enemyStartPos;
	setEnemyPos(enemyPos);
	isOnGround = true;
}

EnemyChar::~EnemyChar()
{

}

void EnemyChar::setEnemyPos(glm::vec3 position)
{
	enemyPos = position;
	this->enemyModelMatrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		enemyPos.x, enemyPos.y, enemyPos.z, 1.0
	);
}

glm::vec3 EnemyChar::getEnemyPos() const
{
	return glm::vec3();
}

glm::mat4 EnemyChar::getModelMatrix() const
{
	return enemyModelMatrix;
}

void EnemyChar::groundCheck()
{
	if (this->enemyPos.y > 0.0f)
	{
		isOnGround = false;
	}
}

void EnemyChar::attack()
{

}

void EnemyChar::update(float dt, glm::vec3 playerPos)
{
	groundCheck();

	//Move
	if (enemyPos.x > playerPos.x)
	{
		enemyPos.x -= 1.0f*dt;
	}
	else
	{
		enemyPos.x += 1.0f*dt;
	}

	if (isOnGround)
	{
		if (fabs(enemyPos.x - playerPos.x) < 0.1)
		{
			velocityY = 10*dt;
		}
	}

	if (!isOnGround)
	{
		velocityY -= 0.5*dt;
	}

	if (velocityY > 5)
	{
		velocityY = 5;
	}

	//Apply velocity
	enemyPos.y += velocityY;

	//Handle collision detection with ground
	if (enemyPos.y < 0) {
		velocityY = 0;
		isOnGround = true;
	}

	setEnemyPos(enemyPos);
}

void EnemyChar::draw(Shader shader)
{
	enemyModel.draw(shader);
}
