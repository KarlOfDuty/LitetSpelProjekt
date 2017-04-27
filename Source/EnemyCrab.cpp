#include "EnemyCrab.h"

EnemyCrab::EnemyCrab(int HP, Model* model, int damage, glm::vec3 enemyStartPos) :EnemyChar(HP, model, damage, enemyStartPos)
{
	this->acceleration = 0.2f;
	this->originPoint = enemyStartPos;
	this->attackNow = true;
}

EnemyCrab::~EnemyCrab()
{

}

void EnemyCrab::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyCrab::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels)
{
	groundCheck();


	if (enemyPosCurrent.x < checkPoint.x-3)
	{
		checkPointReached = true;
	}
	if (enemyPosCurrent.x > checkPoint.x+3)
	{
		checkPointReached = false;
	}

	if (!attackNow)
	{
		if (walkTimer.getElapsedTime().asSeconds() >= 1.5)
		{
			attackNow = true;
		}
	}

	if (attackNow)
	{
		if (glm::length(enemyPosCurrent - originPoint) > 4.0f)
		{
			originPoint = enemyPosCurrent;
			attackNow = false;
			movingRight = false;
			movingLeft = false;
			velocityX = 0;
			walkTimer.restart();
		}
	}

	if (isOnGround)
	{
		//Move
		if (attackNow)
		{
			if (glm::length(enemyPosCurrent - playerPos) < 5.0f || playerSeen)
			{
				if (movingLeft == false)
				{
					if (enemyPosCurrent.x >= playerPos.x)
					{
						movingRight = true;
					}
				}
				if (movingRight == false)
				{
					if (enemyPosCurrent.x <= playerPos.x)
					{
						movingLeft = true;
					}
				}
				if (movingRight == true)
				{
					velocityX = velocityX - acceleration * dt;
				}
				else if (movingLeft == true)
				{
					velocityX = velocityX + acceleration * dt;
				}
				playerSeen = true;
			}
			else
			{
				//Patrol
				if (movingLeft == false)
				{
					if ((!checkPointReached))
					{
						movingRight = true;
					}
				}
				if (movingRight == false)
				{
					if (checkPointReached)
					{
						movingLeft = true;
					}
				}
				if (movingRight == true)
				{
					velocityX = velocityX - acceleration * dt;
				}
				else if (movingLeft == true)
				{
					velocityX = velocityX + acceleration * dt;
				}
			}
		}
	}

	if (!isOnGround)
	{
		velocityY -= 30 * dt;
	}

	if (velocityY > 10)
	{
		velocityY = 10;
	}

	if (velocityY < -10)
	{
		velocityY = -10;
	}

	if (velocityX < -0.3) velocityX = -0.3f;
	if (velocityX > 0.3) velocityX = 0.3f;


	//Apply velocity
	enemyPosCurrent.x += velocityX;
	enemyPosCurrent.y += velocityY*dt;

	//Handle collision detection with ground
	if (enemyPosCurrent.y <= 0) {
		velocityY = 0;
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}

	setEnemyPos(enemyPosCurrent);
}
