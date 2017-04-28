#include "EnemyToad.h"

EnemyToad::EnemyToad(int HP, Model model, int damage, glm::vec3 enemyPos) :EnemyChar(HP, model, damage, enemyPos)
{

}

EnemyToad::~EnemyToad()
{

}

void EnemyToad::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos)
{

}

void EnemyToad::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	//Patrol check - no need to be in toad
	if (fabs(enemyPos.x) < checkPoint.x - 2)
	{
		checkPointReached = true;
	}
	else if (fabs(enemyPos.x) > checkPoint.x + 2)
	{
		checkPointReached = false;
	}

	

	if (isOnGround)
	{
		//Jump
		if (glm::length(enemyPos - playerPos) < 5.0f || playerSeen)
		{
			if (jumpTimer.getElapsedTime().asSeconds() >= 1.4)
			{
				if (isOnGround)
				{
					velocityY = 15;
				}
				jumpTimer.restart();
			}

			playerSeen = true;
		}
		movingRight = false;
		movingLeft = false;
	}

	//Move in air only
	if (!isOnGround)
	{
		if (movingLeft == false)
		{
			if (enemyPos.x >= playerPos.x)
			{
				movingRight = true;
			}
		}
		if (movingRight == false)
		{
			if (enemyPos.x <= playerPos.x)
			{
				movingLeft = true;
			}
		}
		if (movingRight == true)
		{
			velocityX -= 3.0f*dt;
		}
		else if (movingLeft = true)
		{
			velocityX += 3.0f*dt;
		}
		velocityY -= 30 * dt;
	}

	if (velocityY < -30)
	{
		velocityY = -30;
	}
	if (velocityX > 3)
	{
		velocityX = 3;
	}
	if (velocityX < -3)
	{
		velocityX = -3;
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

