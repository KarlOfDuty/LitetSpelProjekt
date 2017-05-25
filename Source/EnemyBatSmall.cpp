#include "EnemyBatSmall.h"
#include "Player.h"
#include "Trigger.h"

EnemyBatSmall::EnemyBatSmall(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor)
{
	goForPlayer = true;
	startPosition = enemyStartPos;
	returnToStart = false;
}

EnemyBatSmall::~EnemyBatSmall()
{

}

void EnemyBatSmall::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBatSmall::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
{
	groundCheck();

	if (collidedFrom.y > 0)
	{
		collidingWithGround = true;
	}
	else if (collidedFrom.y <= 0)
	{
		collidingWithGround = false;
	}

	std::mt19937 rng(rd());
	std::uniform_int_distribution<> distX(-2, 2);
	std::uniform_int_distribution<> distY(0, 3);

	for (int i = 0; i < allSmallBats.size(); i++)
	{
		if (glm::length(enemyPosCurrent - allSmallBats[i]->getPos()) < 2.5f)
		{
			if (playerSeen)
			{
				allSmallBats[i]->playerSeen = true;
			}
		}
	}

	if (collisionWithPlayer(player))
	{
		if (goForPlayer)
		{
			checkpoint.x = player->getPos().x + distX(rng);
			checkpoint.y = player->getPos().y + distY(rng);
			checkpoint.z = player->getPos().z;
			goForPlayer = false;
		}
	}
	if (glm::length(enemyPosCurrent - player->getPos()) > 3.0f)
	{
		goForPlayer = true;
	}

	if (!goForPlayer)
	{
		if (fabs(enemyPosCurrent.x - checkpoint.x) < 0.2f && fabs(enemyPosCurrent.y - checkpoint.y) < 0.2f)
		{
			checkpoint.x = player->getPos().x + distX(rng);
			checkpoint.y = player->getPos().y + distY(rng);
			checkpoint.z = player->getPos().z;
			goForPlayer = true;
		}
	}

	if (collides)
	{
		if (collidedFrom.y > 0 || collidedFrom.x != 0)
		{
			collisionCounter++;
		}
		if (collisionCounter < 300)
		{
			velocityY += 6.0f*dt;
		}

		checkpoint.x = player->getPos().x + distX(rng);
		checkpoint.y = player->getPos().y + distY(rng);
		checkpoint.z = player->getPos().z;
		if (collisionTime.getElapsedTime().asSeconds() >= 5)
		{
			returnToStart = true;
			playerSeen = false;
			goForPlayer = false;
		}
		
		timeSinceCollision.restart();
	}
	else
	{
		collisionTime.restart();
	}

	if (timeSinceCollision.getElapsedTime().asSeconds() >= 1)
	{
		collisionCounter = 0;
	}

	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 12.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	//Move
	if (!returnToStart)
	{
		if (goForPlayer)
		{
			if (playerSeen)
			{
				if (enemyPosCurrent.x > player->getPos().x)
				{
					velocityX -= 3.0f*dt;
				}
				else if (enemyPosCurrent.x < player->getPos().x)
				{
					velocityX += 3.0f*dt;
				}
				if (enemyPosCurrent.y > player->getPos().y)
				{
					velocityY -= 3.0f*dt;
				}
				else if (enemyPosCurrent.y < player->getPos().y)
				{
					velocityY += 3.0f*dt;
				}
				playerSeen = true;
			}
		}
		else
		{
			if (enemyPosCurrent.x > checkpoint.x)
			{
				velocityX -= 3.5f*dt;
			}
			else if (enemyPosCurrent.x < checkpoint.x)
			{
				velocityX += 3.5f*dt;
			}
			if (enemyPosCurrent.y > checkpoint.y)
			{
				velocityY -= 3.5f*dt;
			}
			else if (enemyPosCurrent.y < checkpoint.y)
			{
				velocityY += 3.5f*dt;
			}
		}
	}
	else
	{
		for (int i = 0; i < allSmallBats.size(); i++)
		{
			allSmallBats[i]->playerSeen = false;
		}

			if (glm::length(enemyPosCurrent - startPosition) > 0.5f)
			{
				if (!collides)
				{
					if (enemyPosCurrent.x > startPosition.x)
					{
						velocityX -= 3.0f*dt;
					}
					else if (enemyPosCurrent.x < startPosition.x)
					{
						velocityX += 3.0f*dt;
					}
					if (enemyPosCurrent.y > startPosition.y)
					{
						velocityY -= 3.0f*dt;
					}
					else if (enemyPosCurrent.y < startPosition.y)
					{
						velocityY += 3.0f*dt;
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
				collisionCounter = 0;
				playerSeen = false;
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
	if (enemyPosCurrent.y <= groundPos && !isOnGround)
	{
		if (velocityY < 0)
		{
			enemyPosCurrent.y = groundPos;
			velocityY = 0;
		}
		isOnGround = true;
	}

	setPos(enemyPosCurrent);
	collides = collision(allModels);
}
