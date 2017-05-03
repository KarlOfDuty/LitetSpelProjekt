#include "EnemySlime.h"


EnemySlime::EnemySlime(int health, Model* model, int damage, glm::vec3 enemyStartPos) :Enemy(health, model, damage, enemyStartPos)
{
	startPosition = enemyStartPos;
	returnToStart = false;
}

EnemySlime::~EnemySlime()
{

}

void EnemySlime::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemySlime::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels)
{
	groundCheck();

	//Patrol check 
	if (enemyPosCurrent.x < checkPoint.x-2)
	{
		checkPointReached = true;
		
	}
	else if (enemyPosCurrent.x > checkPoint.x+2)
	{
		checkPointReached = false;

	}

	if (collides)
	{
		if (collidedFrom.x != 0 && collidedFrom.y > 0)
		{
			velocityY = 10;
		}

		if (collidedFrom.x != 0)
		{
			if (collisionTime.getElapsedTime().asSeconds() >= 5)
			{
				returnToStart = true;
				playerSeen = false;
			}
		}
	}
	
	if(collidedFrom.x == 0)
	{
		collisionTime.restart();
	}

	//Move
	if (!returnToStart)
	{
		if (glm::length(enemyPosCurrent - playerPos) < 5.0f || playerSeen)
		{
			if (enemyPosCurrent.x > playerPos.x)
			{
				velocityX -= 2.0f*dt;
			}
			else
			{
				velocityX += 2.0f*dt;
			}
			playerSeen = true;
		}
		else
		{
			//Patrol
			if (checkPointReached == false)
			{
				velocityX -= 2.0f*dt;
			}
			else if (checkPointReached == true)
			{
				velocityX += 2.0f*dt;
			}
		}
	}
	else
	{
		if (glm::length(enemyPosCurrent - startPosition) > 0.5f)
		{
			if (enemyPosCurrent.x > startPosition.x)
			{
				velocityX -= 1.0f*dt;
			}
			else if (enemyPosCurrent.x < startPosition.x)
			{
				velocityX += 1.0f*dt;
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
		if (fabs(enemyPosCurrent.x - playerPos.x) < 0.1)
		{
			velocityY = 10;
		}
	}

	if (!isOnGround)
	{
		velocityY -= 30*dt;
	}

	if (velocityY > 10)
	{
		velocityY = 10;
	}

	if (velocityY < -10)
	{
		velocityY = -10;
	}

	//Apply velocity
	enemyPosCurrent.x += velocityX;
	velocityX = 0;
	enemyPosCurrent.y += velocityY*dt;

	//Handle collision detection with ground
	if (enemyPosCurrent.y <= 0) {
		velocityY = 0;
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}

	setPos(enemyPosCurrent);
	collides = collision(allModels);
}

