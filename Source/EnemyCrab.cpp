#include "EnemyCrab.h"

EnemyCrab::EnemyCrab(int HP, Model model, int damage, glm::vec3 enemyPos) :EnemyChar(HP, model, damage, enemyPos)
{

}

EnemyCrab::~EnemyCrab()
{

}

void EnemyCrab::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos)
{

}

void EnemyCrab::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	//Patrol check 
	if (fabs(enemyPos.x) < checkPoint.x - 2)
	{
		checkPointReached = true;
	}
	else if (fabs(enemyPos.x) > checkPoint.x + 2)
	{
		checkPointReached = false;
	}

	//Move
	if (glm::length(enemyPos - playerPos) < 5.0f || playerSeen)
	{
		if (enemyPos.x > playerPos.x)
		{
			velocityX -= 1.0f*dt;
		}
		else
		{
			velocityX += 1.0f*dt;
		}
		playerSeen = true;
	}
	else
	{
		//Patrol
		if (checkPointReached == false)
		{
			velocityX -= 1.0f*dt;
		}
		else if (checkPointReached == true)
		{
			velocityX += 1.0f*dt;
		}
	}

	if (isOnGround)
	{

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

	//Apply velocity
	enemyPos.x += velocityX;
	velocityX = 0;
	enemyPos.y += velocityY*dt;

	//Handle collision detection with ground
	if (enemyPos.y <= 0) {
		velocityY = 0;
		enemyPos.y = 0;
		isOnGround = true;
	}

	setEnemyPos(enemyPos);
}
