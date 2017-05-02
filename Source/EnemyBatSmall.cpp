#include "EnemyBatSmall.h"

EnemyBatSmall::EnemyBatSmall(int HP, Model* model, int damage, glm::vec3 enemyStartPos) :EnemyChar(HP, model, damage, enemyStartPos)
{
	findPlayer = true;
	startPosition = enemyStartPos;
	returnToStart = false;
}

EnemyBatSmall::~EnemyBatSmall()
{

}

void EnemyBatSmall::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBatSmall::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels)
{
	groundCheck();
	std::mt19937 rng(rd());
	std::uniform_int_distribution<> distX(-2, 2);
	std::uniform_int_distribution<> distY(0, 3);

	for (int i = 0; i < smallBatsPos.size(); i++)
	{
		if (glm::length(enemyPosCurrent - smallBatsPos[i]->getEnemyPos()) < 2.0f)
		{
			if (playerSeen)
			{
				smallBatsPos[i]->playerSeen = true;
			}
		}
	}

	if (fabs(enemyPosCurrent.x - playerPos.x) < 0.5f && fabs(enemyPosCurrent.y - playerPos.y) < 0.5f)
	{
		goForPlayer = false;
		if (findPlayer)
		{
			newCheckpoint.x = playerPos.x + distX(rng);
			newCheckpoint.y = playerPos.y + distY(rng);
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
			newCheckpoint.x = playerPos.x + distX(rng);
			newCheckpoint.y = playerPos.y + distY(rng);
			newCheckpoint.z = playerPos.z;
			goForPlayer = true;
		}
	}

	if (collision)
	{
		velocityY += 5.0f*dt;
		if (collisionClock.getElapsedTime().asSeconds() >= 5)
		{
			returnToStart = true;
			playerSeen = false;
			goForPlayer = true;
			findPlayer = true;
		}
	}
	else
	{
		collisionClock.restart();
	}


	//Move
	if (!returnToStart)
	{
		if (findPlayer || goForPlayer)
		{
			if (glm::length(enemyPosCurrent - playerPos) < 8.0f || playerSeen)
			{
				if (enemyPosCurrent.x > playerPos.x)
				{
					velocityX -= 2.0f*dt;
				}
				else if (enemyPosCurrent.x < playerPos.x)
				{
					velocityX += 2.0f*dt;
				}
				if (enemyPosCurrent.y > playerPos.y)
				{
					velocityY -= 2.0f*dt;
				}
				else if (enemyPosCurrent.y < playerPos.y)
				{
					velocityY += 2.0f*dt;
				}
				playerSeen = true;
			}
		}
		else
		{
			if (enemyPosCurrent.x > newCheckpoint.x)
			{
				velocityX -= 2.2f*dt;
			}
			else if (enemyPosCurrent.x < newCheckpoint.x)
			{
				velocityX += 2.2f*dt;
			}
			if (enemyPosCurrent.y > newCheckpoint.y)
			{
				velocityY -= 2.2f*dt;
			}
			else if (enemyPosCurrent.y < newCheckpoint.y)
			{
				velocityY += 2.2f*dt;
			}
		}
	}
	else
	{
		playerSeen = false;

			if (glm::length(enemyPosCurrent - startPosition) > 0.5f)
			{
				if (!collision)
				{
					if (enemyPosCurrent.x > startPosition.x)
					{
						velocityX -= 2.0f*dt;
					}
					else if (enemyPosCurrent.x < startPosition.x)
					{
						velocityX += 2.0f*dt;
					}
					if (enemyPosCurrent.y > startPosition.y)
					{
						velocityY -= 2.0f*dt;
					}
					else if (enemyPosCurrent.y < startPosition.y)
					{
						velocityY += 2.0f*dt;
					}
				}
				else
				{
					velocityY += 5.0f*dt;
				}
			}
			else
			{
				returnToStart = false;
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
	collision = checkCollision(allModels);
}
