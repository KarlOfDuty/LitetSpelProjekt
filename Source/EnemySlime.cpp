#include "EnemySlime.h"

EnemySlime::EnemySlime(int HP, Model model, int damage, glm::vec3 enemyPos, glm::vec3 checkPoint) :EnemyChar(HP, model, damage, enemyPos, checkPoint)
{
	
}

EnemySlime::~EnemySlime()
{

}

void EnemySlime::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos)
{

}

void EnemySlime::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos)
{
	groundCheck();
	std::cout << fabs(enemyPos.x) << std::endl;
	if (fabs(enemyPos.x) < )
	{
		checkPointReached = true;
	}
	else if (fabs(enemyPos.x) > 8.0f)
	{
		checkPointReached = false;
	}

	//Move
	if (glm::length(enemyPos - playerPos) < 5.0f)
	{
		if (enemyPos.x > playerPos.x)
		{
			enemyPos.x -= 1.0f*dt;
		}
		else
		{
			enemyPos.x += 1.0f*dt;
		}
	}
	else
	{
		if(checkPointReached == false)
		{
			enemyPos.x -= 1.0f*dt;
		}
		else if (checkPointReached == true)
		{
			enemyPos.x += 1.0f*dt;
		}
	}

	if (isOnGround)
	{
		if (fabs(enemyPos.x - playerPos.x) < 0.1)
		{
			velocityY = 10 * dt;
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
		enemyPos.y = 0;
		isOnGround = true;
	}

	setEnemyPos(enemyPos);
}

