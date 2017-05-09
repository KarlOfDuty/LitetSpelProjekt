#include "EnemyBat.h"

EnemyBat::EnemyBat(int health, Model* model, int damage, glm::vec3 enemyStartPos) :Enemy(health, model, damage, enemyStartPos)
{
	swoopAttack = true;
	startPosition = enemyStartPos;
	returnToStart = false;
	newCheckPoint = enemyStartPos;
}

EnemyBat::~EnemyBat()
{

}

void EnemyBat::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBat::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, std::vector<glm::vec2> playerPoints)
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

	if (enemyPosCurrent.y <= 3)
	{
		goUp = true;
	}

	if (fabs(enemyPosCurrent.y - newCheckPoint.y) < 1.0f)
	{
		checkPointReached = true;
		goingLeft = false;
		goingRight = false;
		goUp = false;
		swoopAttack = false;
	}
	else
	{
		waitInAir.restart();
		checkPointReached = false;
	}

	if (playerSeen)
	{
		if (checkPointReached)
		{
			if (enemyPosCurrent.x > playerPos.x)
			{
				goingLeft = true;
			}
			else if (enemyPosCurrent.x < playerPos.x)
			{
				goingRight = true;
			}

			if (goingLeft)
			{
				newCheckPoint.x = newCheckPoint.x - 10.0f;
				newCheckPoint.y = startPosition.y;
				newCheckPoint.z = startPosition.z;
			}

			if (goingRight)
			{
				newCheckPoint.x = newCheckPoint.x + 10.0f;
				newCheckPoint.y = startPosition.y;
				newCheckPoint.z = startPosition.z;
			}
			
		}
	}

	if (waitInAir.getElapsedTime().asSeconds() >= 1.2)
	{
		swoopAttack = true;
	}


	if (collides)
	{
		if (collidedFrom.y == 0 && collidedFrom.x != 0 || collidedFrom.y > 0 && collidedFrom.x == 0)
		{
			velocityY += 2.0f*dt;
		}
		else if (collidedFrom.y < 0)
		{
			newCheckPoint.y = enemyPosCurrent.y;
			newCheckPoint.x = enemyPosCurrent.x;
		}

		if (collisionTime.getElapsedTime().asSeconds() >= 5)
		{
			returnToStart = true;
			playerSeen = false;
		}
	}
	else
	{
		collisionTime.restart();
	}

	//Detect player
	if (glm::length(enemyPosCurrent - playerPos) < 15.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	//Move
	midX = startPosition.y-13;
	float curve = glm::pow(enemyPosCurrent.y - midX, 0.5);
	if (!returnToStart)
	{
		if (swoopAttack)
		{
			if (playerSeen)
			{
				if (fabs(enemyPosCurrent.x - newCheckPoint.x) > 2.0f && goingLeft && !goUp)
				{
					velocityX -= 4.0f*dt;
					velocityY -= curve*dt;
				}
				else if (fabs(enemyPosCurrent.x - newCheckPoint.x) > 2.0f && goingRight && !goUp)
				{
					velocityX += 4.0f*dt;
					velocityY -= curve*dt;
				}
				if (fabs(enemyPosCurrent.x - newCheckPoint.x) > 2.0f && goingLeft && goUp)
				{
					velocityX -= 4.0f*dt;
					velocityY += curve*dt;
				}
				else if (fabs(enemyPosCurrent.x - newCheckPoint.x) > 2.0f && goingRight && goUp)
				{
					velocityX += 4.0f*dt;
					velocityY += curve*dt;
				}
				if (enemyPosCurrent.y > newCheckPoint.y)
				{
					velocityY -= curve*dt;
				}
			}
		}
	}
	else
	{
		if (glm::length(enemyPosCurrent - startPosition) > 0.5f)
		{
			if (!collides)
			{
				if (enemyPosCurrent.x > startPosition.x)
				{
					velocityX -= 2.5f*dt;
				}
				else if (enemyPosCurrent.x < startPosition.x)
				{
					velocityX += 2.5f*dt;
				}
				if (enemyPosCurrent.y > startPosition.y)
				{
					velocityY -= 2.5f*dt;
				}
				else if (enemyPosCurrent.y < startPosition.y)
				{
					velocityY += 2.5f*dt;
				}
			}
			else
			{
				velocityY += 2.0f*dt;
			}
		}
		else
		{
			returnToStart = false;
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

