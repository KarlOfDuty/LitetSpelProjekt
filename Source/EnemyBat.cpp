#include "EnemyBat.h"

EnemyBat::EnemyBat(int HP, Model model, int damage, glm::vec3 enemyPos) :EnemyChar(HP, model, damage, enemyPos)
{
	findPlayer = true;
}

EnemyBat::~EnemyBat()
{

}

void EnemyBat::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos)
{

}

void EnemyBat::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	if (findPlayer)
	{
		if (fabs(enemyPos.x - playerPos.x) < 0.2 && fabs(enemyPos.y - playerPos.y) < 0.3)
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
		if (fabs(enemyPos.x - newCheckpoint.x) < 0.2 && fabs(enemyPos.y - newCheckpoint.y) < 0.3)
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
		if (glm::length(enemyPos - playerPos) < 10.0f || playerSeen)
		{
			if (enemyPos.x > playerPos.x)
			{
				velocityX -= 1.7f*dt;
				goingLeft = true;
				goingRight = false;
			}
			else if (enemyPos.x < playerPos.x)
			{
				velocityX += 1.7f*dt;
				goingRight = true;
				goingLeft = false;
			}
			if (enemyPos.y > playerPos.y)
			{
				velocityY -= 1.7f*dt;
			}
			else if (enemyPos.y < playerPos.y)
			{
				velocityY += 1.7f*dt;
			}
			playerSeen = true;
		}
	}
	else
	{
		if (enemyPos.x > newCheckpoint.x)
		{
			velocityX -= 1.7f*dt;
		}
		else if (enemyPos.x < newCheckpoint.x)
		{
			velocityX += 1.7f*dt;
		}
		if (enemyPos.y > newCheckpoint.y)
		{
			velocityY -= 1.7f*dt;
		}
		else if (enemyPos.y < newCheckpoint.y)
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
	enemyPos.x += velocityX;
	velocityX = 0;
	enemyPos.y += velocityY;
	velocityY = 0;

	//Handle collision detection with ground
	if (enemyPos.y <= 0) {
		enemyPos.y = 0;
		isOnGround = true;
	}

	setEnemyPos(enemyPos);
}

