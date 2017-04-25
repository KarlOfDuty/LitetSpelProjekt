#include "EnemyBoss.h"

EnemyBoss::EnemyBoss(int HP, Model model, int damage, glm::vec3 enemyPosCurrent) :EnemyChar(HP, model, damage, enemyPosCurrent)
{
	findPlayer = true;
}

EnemyBoss::~EnemyBoss()
{

}

void EnemyBoss::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBoss::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	if (findPlayer)
	{
		if (fabs(enemyPosCurrent.x - playerPos.x) < 0.2 && fabs(enemyPosCurrent.y - playerPos.y) < 0.3)
		{
			if (goingRight)
			{
				newCheckpoint.x = playerPos.x + 7;
				newCheckpoint.y = playerPos.y + 6;
				newCheckpoint.z = playerPos.z;
			}
			if (goingLeft)
			{
				newCheckpoint.x = playerPos.x - 7;
				newCheckpoint.y = playerPos.y + 6;
				newCheckpoint.z = playerPos.z;
			}
			findPlayer = false;
			clockRestart = true;
		}
	}

	if (!findPlayer)
	{
		if (fabs(enemyPosCurrent.x - newCheckpoint.x) < 0.2 && fabs(enemyPosCurrent.y - newCheckpoint.y) < 0.3)
		{
			if (clockRestart)
			{
				waitInAir.restart();
				clockRestart = false;
			}
			if (waitInAir.getElapsedTime().asSeconds() >= 1.0)
			{
				findPlayer = true;
			}
		}
	}

	//Move
	if (findPlayer == true)
	{
		if (glm::length(enemyPosCurrent - playerPos) < 10.0f || playerSeen)
		{
			if (enemyPosCurrent.x > playerPos.x)
			{
				velocityX -= 1.7f*dt;
				goingLeft = true;
				goingRight = false;
			}
			else if (enemyPosCurrent.x < playerPos.x)
			{
				velocityX += 1.7f*dt;
				goingRight = true;
				goingLeft = false;
			}
			if (enemyPosCurrent.y > playerPos.y)
			{
				velocityY -= 1.7f*dt;
			}
			else if (enemyPosCurrent.y < playerPos.y)
			{
				velocityY += 1.7f*dt;
			}
			playerSeen = true;
		}
	}
	else
	{
		if (enemyPosCurrent.x > newCheckpoint.x)
		{
			velocityX -= 1.7f*dt;
		}
		else if (enemyPosCurrent.x < newCheckpoint.x)
		{
			velocityX += 1.7f*dt;
		}
		if (enemyPosCurrent.y > newCheckpoint.y)
		{
			velocityY -= 1.7f*dt;
		}
		else if (enemyPosCurrent.y < newCheckpoint.y)
		{
			velocityY += 1.7f*dt;
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

	setEnemyPos(enemyPosCurrent);
}
