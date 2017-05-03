#include "EnemyFireFly.h"

EnemyFireFly::EnemyFireFly(int health, Model* model, int damage, glm::vec3 enemyStartPos) :Enemy(health, model, damage, enemyStartPos)
{
	this->attackRange = 8;
	this->startPosition = enemyStartPos;
}

EnemyFireFly::~EnemyFireFly()
{

}

void EnemyFireFly::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyFireFly::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels)
{
	groundCheck();

	if (collides)
	{
		velocityY += 3.0f*dt;

		if (collisionTime.getElapsedTime().asSeconds() >= 5)
		{
			returnToStart = true;
			playerSeen = false;
		}
	}
	else
	{
		collisionTime.restart();
	}

	//Move
	if (!returnToStart)
	{
		if (glm::length(enemyPosCurrent - playerPos) < 10.0f || playerSeen)
		{
			if (enemyPosCurrent.x > playerPos.x + attackRange)
			{
				velocityX -= 1.5f*dt;
			}
			else if (enemyPosCurrent.x < playerPos.x - attackRange)
			{
				velocityX += 1.5f*dt;
			}
			if (enemyPosCurrent.y > playerPos.y)
			{
				velocityY -= 1.5f*dt;
			}
			else if (enemyPosCurrent.y < playerPos.y)
			{
				velocityY += 1.5f*dt;
			}
			playerSeen = true;
		}
	}
	else
	{
		if (glm::length(enemyPosCurrent - startPosition) > 0.5f)
		{
			if (!collides)
			{
				if (enemyPosCurrent.x > startPosition.x)
				{
					velocityX -= 1.5f*dt;
				}
				else if (enemyPosCurrent.x < startPosition.x)
				{
					velocityX += 1.5f*dt;
				}
				if (enemyPosCurrent.y > startPosition.y)
				{
					velocityY -= 1.5f*dt;
				}
				else if (enemyPosCurrent.y < startPosition.y)
				{
					velocityY += 1.5f*dt;
				}
			}
			else
			{
				velocityY += 3.0f*dt;
			}
		}
		else
		{
			returnToStart = false;
			playerSeen = false;
		}
	}


	if (isOnGround)
	{

	}

	if (!isOnGround)
	{

	}

	//Apply velocity
	enemyPosCurrent.x += velocityX;
	velocityX = 0;
	enemyPosCurrent.y += velocityY;
	velocityY = 0;

	//Handle collision detection with ground
	if (enemyPosCurrent.y <= 0) {
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}

	setPos(enemyPosCurrent);
	collides = collision(allModels);
}
