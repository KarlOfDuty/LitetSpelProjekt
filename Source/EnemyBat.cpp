#include "EnemyBat.h"
#include "Player.h"

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

void EnemyBat::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
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

	if (collides)
	{
		if (collidedFrom.y == 0 && collidedFrom.x != 0)
		{
			velocityY += 7.0f*dt;
		}
		if (collidedFrom.y > 0)
		{
			if (enemyPosCurrent.x > player->getPos().x)
			{
				velocityY += 7.0f*dt;
				velocityX -= 7.0f*dt;
			}
			else if (enemyPosCurrent.x < player->getPos().x)
			{
				velocityY += 7.0f*dt;
				velocityX += 7.0f*dt;
			}
		}

		if (collidedFrom.y < 0)
		{
			newCheckPoint.y = enemyPosCurrent.y;
			newCheckPoint.x = enemyPosCurrent.x;
		}

		if (collisionTime.getElapsedTime().asSeconds() >= 6)
		{
			returnToStart = true;
			playerSeen = false;
		}
	}
	else
	{
		collisionTime.restart();
	}

	if (enemyPosCurrent.y <= 3)
	{
		goUp = true;
	}

	if (fabs(enemyPosCurrent.x - newCheckPoint.x) < 0.5f)
	{
		xPointReached = true;
	}

	if (fabs(enemyPosCurrent.y - newCheckPoint.y) < 0.5f)
	{
		yPointReached = true;
	}

	if (yPointReached && !xPointReached)
	{
		xPointReached = true;
	}

	if (xPointReached && yPointReached)
	{
		checkPointReached = true;
		goingLeft = false;
		goingRight = false;
		goUp = false;
		swoopAttack = false;
	}
	
	if (!xPointReached && !yPointReached)
	{
		checkPointReached = false;
		waitInAir.restart();
	}

	if (playerSeen)
	{
		if (waitInAir.getElapsedTime().asSeconds() >= 1.2)
		{
			if (checkPointReached)
			{
				if (enemyPosCurrent.x > player->getPos().x)
				{
					goingLeft = true;
				}
				else if (enemyPosCurrent.x < player->getPos().x)
				{
					goingRight = true;
				}

				if (goingLeft)
				{
					newCheckPoint.x = newCheckPoint.x - 20.0f;
					newCheckPoint.y = startPosition.y;
					newCheckPoint.z = startPosition.z;
				}

				if (goingRight)
				{
					newCheckPoint.x = newCheckPoint.x + 20.0f;
					newCheckPoint.y = startPosition.y;
					newCheckPoint.z = startPosition.z;
				}
				swoopAttack = true;
				xPointReached = false;
				yPointReached = false;
			}
		}
	}

	

	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 15.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	//Move
	midX = startPosition.y-13;
	float curve = glm::pow(enemyPosCurrent.y - midX, 0.7);
	if (!returnToStart)
	{
		if (swoopAttack)
		{
			if (playerSeen)
			{
				if (fabs(enemyPosCurrent.x - newCheckPoint.x) > 0.1f && goingLeft && !goUp)
				{
					velocityX -= 5.0f*dt;
					velocityY -= curve*dt;
				}
				else if (fabs(enemyPosCurrent.x - newCheckPoint.x) > 0.1f && goingRight && !goUp)
				{
					velocityX += 5.0f*dt;
					velocityY -= curve*dt;
				}
				if (fabs(enemyPosCurrent.y - newCheckPoint.y) > 0.1f && goingLeft && goUp)
				{
					velocityX -= 5.0f*dt;
					velocityY += curve*dt;
				}
				else if (fabs(enemyPosCurrent.y - newCheckPoint.y) > 0.1f && goingRight && goUp)
				{
					velocityX += 5.0f*dt;
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

