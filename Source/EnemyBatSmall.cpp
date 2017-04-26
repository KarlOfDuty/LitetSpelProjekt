#include "EnemyBatSmall.h"

EnemyBatSmall::EnemyBatSmall(int HP, Model model, int damage, glm::vec3 enemyStartPos) :EnemyChar(HP, model, damage, enemyStartPos)
{
	std::srand(time(0));
	findPlayer = true;
}

EnemyBatSmall::~EnemyBatSmall()
{

}

float EnemyBatSmall::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

void EnemyBatSmall::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBatSmall::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	for (int i = 0; i < smallBatsPos.size(); i++)
	{
		if (glm::length(enemyPosCurrent - smallBatsPos[i]->getEnemyPos()) < 5.0f)
		{
			if (playerSeen)
			{
				smallBatsPos[i]->playerSeen = true;
			}
		}
	}

	if(fabs(enemyPosCurrent.x - playerPos.x) < 0.5f && fabs(enemyPosCurrent.y - playerPos.y) < 0.5f)
	{
		goForPlayer = false;
		if (findPlayer)
		{
			newCheckpoint.x = playerPos.x + RandomNumber(-2.0f, 2.0f);
			newCheckpoint.y = playerPos.y + RandomNumber(-1.0f, 2.0f);
			newCheckpoint.z = playerPos.z;
			findPlayer = false;
		}
	}
	if (glm::length(enemyPosCurrent - playerPos) > 3.0f)
	{
		findPlayer = true;
	}

	if (!findPlayer && !goForPlayer)
	{
		if (fabs(enemyPosCurrent.x - newCheckpoint.x) < 0.1f && fabs(enemyPosCurrent.y - newCheckpoint.y) < 0.1f)
		{
			newCheckpoint.x = playerPos.x + RandomNumber(-2.0f, 2.0f);
			newCheckpoint.y = playerPos.y + RandomNumber(-1.0f, 2.0f);
			newCheckpoint.z = playerPos.z;
			goForPlayer = true;
		}
	}


	
	//Move
	if (findPlayer || goForPlayer)
	{
		if (glm::length(enemyPosCurrent - playerPos) < 10.0f || playerSeen)
		{
			if (enemyPosCurrent.x > playerPos.x)
			{
				velocityX -= 1.8f*dt;
			}
			else if (enemyPosCurrent.x < playerPos.x)
			{
				velocityX += 1.8f*dt;
			}
			if (enemyPosCurrent.y > playerPos.y)
			{
				velocityY -= 1.8f*dt;
			}
			else if (enemyPosCurrent.y < playerPos.y)
			{
				velocityY += 1.8f*dt;
			}
			playerSeen = true;
		}
	}
	else
	{
		if (enemyPosCurrent.x > newCheckpoint.x)
		{
			velocityX -= 1.8f*dt;
		}
		else if (enemyPosCurrent.x < newCheckpoint.x)
		{
			velocityX += 1.8f*dt;
		}
		if (enemyPosCurrent.y > newCheckpoint.y)
		{
			velocityY -= 1.8f*dt;
		}
		else if (enemyPosCurrent.y < newCheckpoint.y)
		{
			velocityY += 1.8f*dt;
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
