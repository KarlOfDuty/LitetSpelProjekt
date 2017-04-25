#include "EnemyToad.h"

EnemyToad::EnemyToad(int HP, Model model, int damage, glm::vec3 enemyPosCurrent) :EnemyChar(HP, model, damage, enemyPosCurrent)
{

}

EnemyToad::~EnemyToad()
{

}

void EnemyToad::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyToad::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	//Patrol check - no need to be in toad
	if (fabs(enemyPosCurrent.x) < checkPoint.x - 2)
	{
		checkPointReached = true;
	}
	else if (fabs(enemyPosCurrent.x) > checkPoint.x + 2)
	{
		checkPointReached = false;
	}

	

	if (isOnGround)
	{
		//Jump
		if (glm::length(enemyPosCurrent - playerPos) < 5.0f || playerSeen)
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
			velocityX -= 3.0f*dt;
		}
		else if (movingLeft == true)
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
	enemyPosCurrent.x += velocityX;
	velocityX = 0;
	enemyPosCurrent.y += velocityY*dt;

	//Handle collision detection with ground
	if (enemyPosCurrent.y <= 0) {
		velocityY = 0;
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}

	setEnemyPos(enemyPosCurrent);
}

