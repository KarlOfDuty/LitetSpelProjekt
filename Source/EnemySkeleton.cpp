#include "EnemySkeleton.h"

EnemySkeleton::EnemySkeleton(int HP, Model model, int damage, bool patrol, glm::vec3 enemyPosCurrent) :EnemyChar(HP, model, damage, enemyPosCurrent)
{
	this->patrol = patrol;
}

EnemySkeleton::~EnemySkeleton()
{

}

void EnemySkeleton::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemySkeleton::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	//Patrol check 
	if (patrol)
	{
		if (fabs(enemyPosCurrent.x) < checkPoint.x - 2)
		{
			checkPointReached = true;
		}
		else if (fabs(enemyPosCurrent.x) > checkPoint.x + 2)
		{
			checkPointReached = false;
		}
	}

	//Move
	if (glm::length(enemyPosCurrent - playerPos) < 5.0f || playerSeen)
	{
		if (enemyPosCurrent.x > playerPos.x)
		{
			velocityX -= 3.0f*dt;
		}
		else
		{
			velocityX += 3.0f*dt;
		}
		playerSeen = true;
	}
	else
	{
		//Patrol
		if (patrol)
		{
			if (checkPointReached == false)
			{
				velocityX -= 1.0f*dt;
			}
			else if (checkPointReached == true)
			{
				velocityX += 1.0f*dt;
			}
		}
	}

	if (isOnGround)
	{
		if (fabs(enemyPosCurrent.x - playerPos.x) < 0.1)
		{
			velocityY = 10 * dt;
		}
	}

	if (!isOnGround)
	{
		velocityY -= 0.7*dt;
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
	enemyPosCurrent.y += velocityY;

	//Handle collision detection with ground
	if (enemyPosCurrent.y <= 0) {
		velocityY = 0;
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}

	setEnemyPos(enemyPosCurrent);
}
