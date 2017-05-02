#include "EnemyBat.h"

EnemyBat::EnemyBat(int HP, Model* model, int damage, glm::vec3 enemyStartPos) :EnemyChar(HP, model, damage, enemyStartPos)
{
	findPlayer = true;
}

EnemyBat::~EnemyBat()
{

}

void EnemyBat::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBat::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels)
{
	groundCheck();

	if (goingRight)
	{
		newCheckpoint.x = playerPos.x + 12.0f;
		newCheckpoint.y = playerPos.y + 7.0f;
		newCheckpoint.z = playerPos.z;
	}
	if (goingLeft)
	{
		newCheckpoint.x = playerPos.x - 12.0f;
		newCheckpoint.y = playerPos.y + 7.0f;
		newCheckpoint.z = playerPos.z;
	}

	if (findPlayer)
	{
		if (fabs(enemyPosCurrent.x - playerPos.x) < 2.0f && fabs(enemyPosCurrent.y - playerPos.y) < 2.0f)
		{
			findPlayer = false;
			clockRestart = true;
		}
	}

	if (!findPlayer)
	{
		if (fabs(enemyPosCurrent.x - newCheckpoint.x) < 5.0f && fabs(enemyPosCurrent.y - newCheckpoint.y) < 5.0f)
		{
			if (clockRestart)
			{
				waitInAir.restart();
				clockRestart = false;
			}
			if (waitInAir.getElapsedTime().asSeconds() >= 0.9)
			{
				findPlayer = true;
			}
		}
	}

	//Move
	midX = playerPos.y;
	yValue = glm::pow(enemyPosCurrent.y - midX, 0.3);
	if (findPlayer == true)
	{
		if (glm::length(enemyPosCurrent - playerPos) < 10.0f || playerSeen)
		{
			if (enemyPosCurrent.x > playerPos.x)
			{
				velocityX -= 2.6f*dt;
				velocityY -= yValue*dt;
				goingLeft = true;
				goingRight = false;
			}
			else if (enemyPosCurrent.x < playerPos.x)
			{
				velocityX += 2.6f*dt;
				velocityY -= yValue*dt;
				goingRight = true;
				goingLeft = false;
			}
			playerSeen = true;
		}
	}
	else
	{
		if (enemyPosCurrent.x > newCheckpoint.x)
		{
			velocityX -= 2.6f*dt;
		}
		else if (enemyPosCurrent.x < newCheckpoint.x)
		{
			velocityX += 2.6f*dt;
			
		}
		if (enemyPosCurrent.y > newCheckpoint.y)
		{
			velocityY -= yValue*dt;
		}
		else if (enemyPosCurrent.y < newCheckpoint.y)
		{
			velocityY += yValue*dt;
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
	/*if (enemyPosCurrent.y <= 0) {
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}*/

	setEnemyPos(enemyPosCurrent);
	checkCollision(allModels);
}

