#include "EnemyBat.h"

EnemyBat::EnemyBat(int health, Model* model, int damage, glm::vec3 enemyStartPos) :Enemy(health, model, damage, enemyStartPos)
{
	swoopAttack = true;
	startPosition = enemyStartPos;
	returnToStart = false;
}

EnemyBat::~EnemyBat()
{

}

void EnemyBat::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBat::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels)
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

	if (goingRight)
	{
		checkpoint.x = playerPos.x + 8.0f;
		checkpoint.y = playerPos.y + 7.0f;
		checkpoint.z = playerPos.z;
	}
	if (goingLeft)
	{
		checkpoint.x = playerPos.x - 8.0f;
		checkpoint.y = playerPos.y + 7.0f;
		checkpoint.z = playerPos.z;
	}

	if (swoopAttack)
	{
		if (fabs(enemyPosCurrent.x - playerPos.x) < 2.0f && fabs(enemyPosCurrent.y - playerPos.y) < 2.0f)
		{
			swoopAttack = false;
			clockRestart = true;
		}
	}

	if (!swoopAttack)
	{
		if (fabs(enemyPosCurrent.x - checkpoint.x) < 1.5f && fabs(enemyPosCurrent.y - checkpoint.y) < 1.5f)
		{
			goingLeft = false;
			goingRight = false;
			if (clockRestart)
			{
				waitInAir.restart();
				clockRestart = false;
			}
			if (waitInAir.getElapsedTime().asSeconds() >= 1.2)
			{
				swoopAttack = true;
			}
		}
	}

	if (collides)
	{
		velocityY += 2.0f*dt;

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

	if (glm::length(enemyPosCurrent - playerPos) < 10.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	//Move
	midX = playerPos.y;
	float curve = glm::pow(enemyPosCurrent.y - midX, 0.7);
	if (!returnToStart)
	{
		if (swoopAttack == true)
		{
			if (playerSeen)
			{
				if (enemyPosCurrent.x > playerPos.x)
				{
					velocityX -= 4.0f*dt;
					velocityY -= curve*dt;
					if (!goingRight)
					{
						goingLeft = true;
					}
				}
				else if (enemyPosCurrent.x < playerPos.x)
				{
					velocityX += 4.0f*dt;
					velocityY -= curve*dt;
					if (!goingLeft)
					{
						goingRight = true;
					}
				}
				playerSeen = true;
			}
		}
		else
		{
			if (enemyPosCurrent.x > checkpoint.x)
			{
				velocityX -= 4.0f*dt;
				velocityY += curve*dt;
			}
			else if (enemyPosCurrent.x < checkpoint.x)
			{
				velocityX += 4.0f*dt;
				velocityY += curve*dt;
			}
			if (enemyPosCurrent.y > checkpoint.y)
			{
				velocityY -= curve*dt;
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

