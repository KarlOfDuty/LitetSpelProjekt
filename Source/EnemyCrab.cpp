#include "EnemyCrab.h"

EnemyCrab::EnemyCrab(int HP, Model model, int damage, glm::vec3 enemyPos) :EnemyChar(HP, model, damage, enemyPos)
{
	acceleration = 0.2f;
}

EnemyCrab::~EnemyCrab()
{

}

void EnemyCrab::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos)
{

}

void EnemyCrab::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
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

	if (walkTimer.getElapsedTime().asSeconds() >= 2.0)
	{
		attackNow = true;
		originPoint = enemyPosCurrent;
		walkTimer.restart();
	}

	if (glm::length(enemyPosCurrent - originPoint) > 4.0f)
	{
		attackNow = false;
		movingRight = false;
		movingLeft = false;
		velocityX = 0;
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
