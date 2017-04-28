#include "EnemyBatSmall.h"

EnemyBatSmall::EnemyBatSmall(int HP, Model model, int damage, glm::vec3 enemyPos) :EnemyChar(HP, model, damage, enemyPos)
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

void EnemyBatSmall::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos)
{

}

void EnemyBatSmall::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	for (int i = 0; i < smallBatsPos.size(); i++)
	{
		if (glm::length(enemyPos - smallBatsPos[i]->getEnemyPos()) < 5.0f)
		{
			if (playerSeen)
			{
				smallBatsPos[i]->playerSeen = true;
			}
		}
	}

	if(fabs(enemyPos.x - playerPos.x) < 0.5f && fabs(enemyPos.y - playerPos.y) < 0.5f)
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
	if (glm::length(enemyPos - playerPos) > 3.0f)
	{
		findPlayer = true;
	}

	if (!findPlayer && !goForPlayer)
	{
		if (fabs(enemyPos.x - newCheckpoint.x) < 0.1f && fabs(enemyPos.y - newCheckpoint.y) < 0.1f)
		{
			newCheckpoint.x = playerPos.x + RandomNumber(-2.0f, 2.0f);
			newCheckpoint.y = playerPos.y + RandomNumber(-1.0f, 2.0f);
			newCheckpoint.z = playerPos.z;
			std::cout << RandomNumber(-2.0f, 2.0f) << std::endl;
			goForPlayer = true;
		}
	}


	
	//Move
	if (findPlayer || goForPlayer)
	{
		if (glm::length(enemyPos - playerPos) < 10.0f || playerSeen)
		{
			if (enemyPos.x > playerPos.x)
			{
				velocityX -= 1.8f*dt;
			}
			else if (enemyPos.x < playerPos.x)
			{
				velocityX += 1.8f*dt;
			}
			if (enemyPos.y > playerPos.y)
			{
				velocityY -= 1.8f*dt;
			}
			else if (enemyPos.y < playerPos.y)
			{
				velocityY += 1.8f*dt;
			}
			playerSeen = true;
		}
	}
	else
	{
		if (enemyPos.x > newCheckpoint.x)
		{
			velocityX -= 1.8f*dt;
		}
		else if (enemyPos.x < newCheckpoint.x)
		{
			velocityX += 1.8f*dt;
		}
		if (enemyPos.y > newCheckpoint.y)
		{
			velocityY -= 1.8f*dt;
		}
		else if (enemyPos.y < newCheckpoint.y)
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
