#include "EnemySlime.h"
#include "Player.h"


EnemySlime::EnemySlime(int health, Model* model, int damage, glm::vec3 enemyStartPos) :Enemy(health, model, damage, enemyStartPos)
{
	startPosition = enemyStartPos;
	returnToStart = false;
}

EnemySlime::~EnemySlime()
{

}

void EnemySlime::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemySlime::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
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

	//Patrol check 
	if (enemyPosCurrent.x < checkPoint.x-2)
	{
		checkPointReached = true;
		
	}
	else if (enemyPosCurrent.x > checkPoint.x+2)
	{
		checkPointReached = false;
	}

	if (collides)
	{
		if (collidedFrom.x != 0 && collidedFrom.y > 0)
		{
			velocityY = 10;
		}

		if (collidedFrom.x != 0)
		{
			if (collisionTime.getElapsedTime().asSeconds() >= 5)
			{
				returnToStart = true;
				playerSeen = false;
			}
		}
	}
	
	if(collidedFrom.x == 0)
	{
		collisionTime.restart();
	}

	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 5.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	//Move
	if (!returnToStart)
	{
		if (playerSeen)
		{
			if (enemyPosCurrent.x > player->getPos().x)
			{
				velocityX -= 2.0f*dt;
			}
			else
			{
				velocityX += 2.0f*dt;
			}
			playerSeen = true;
		}
		else
		{
			//Patrol
			/*if (checkPointReached == false)
			{
				velocityX -= 2.0f*dt;
			}
			else if (checkPointReached == true)
			{
				velocityX += 2.0f*dt;
			}*/
		}
	}
	else
	{
		if (glm::length(enemyPosCurrent.x - startPosition.x) > 0.5f)
		{
			if (enemyPosCurrent.x > startPosition.x)
			{
				velocityX -= 1.0f*dt;
			}
			else if (enemyPosCurrent.x < startPosition.x)
			{
				velocityX += 1.0f*dt;
			}
		}
		else
		{
			returnToStart = false;
			playerSeen = false;
		}
	}

	if (collidingWithGround)
	{
		if (collisionWithPlayer(player))
		{
			velocityY = 10;
		}
	}

	if (!collidingWithGround)
	{
		velocityY -= 30*dt;
	}

	if (velocityY > 10)
	{
		velocityY = 10;
	}

	if (velocityY < -10)
	{
		velocityY = -10;
	}

	//Apply velocity
	enemyPosCurrent.x += velocityX;
	velocityX = 0;
	enemyPosCurrent.y += velocityY*dt;

	//Handle collision detection with ground
	if (enemyPosCurrent.y <= groundPos)
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

