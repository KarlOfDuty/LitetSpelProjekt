#include "EnemySkeleton.h"

EnemySkeleton::EnemySkeleton(int health, Model* model, int damage, bool patrol, glm::vec3 enemyStartPos) :Enemy(health, model, damage, enemyStartPos)
{
	this->patrol = patrol;
	std::srand(time(0));
	this->startPosition = enemyStartPos;
}

EnemySkeleton::~EnemySkeleton()
{

}

void EnemySkeleton::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{
	//randomize doValue, if above 3 attack, if 1/2/3 block
	int doValue = rand() % 10 + 1;
	if (doValue >= 4)
	{
		//attackPlayer
	}
	else if (doValue <= 3)
	{
		//block, or dodge
	}
}

void EnemySkeleton::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*>& allModels, std::vector<glm::vec2> playerPoints)
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
	if (patrol)
	{
		if (enemyPosCurrent.x < checkPoint.x - 3)
		{
			checkPointReached = true;
		}
		else if (enemyPosCurrent.x > checkPoint.x + 3)
		{
			checkPointReached = false;
		}
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

	if (collidedFrom.x == 0)
	{
		collisionTime.restart();
	}

	//Detect player
	if (glm::length(enemyPosCurrent - playerPos) < 5.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	if (!returnToStart)
	{
		//Move
		if (playerSeen)
		{
			if (enemyPosCurrent.x > playerPos.x)
			{
				velocityX -= 3.8f*dt;
			}
			else
			{
				velocityX += 3.8f*dt;
			}
		}
		else
		{
			//Patrol
			if (patrol)
			{
				if (checkPointReached == false)
				{
					velocityX -= 1.8f*dt;
				}
				else if (checkPointReached == true)
				{
					velocityX += 1.8f*dt;
				}
			}
		}
	}
	else
	{
		if (glm::length(enemyPosCurrent.x - startPosition.x) > 0.5f)
		{
			if (enemyPosCurrent.x > startPosition.x)
			{
				velocityX -= 1.8f*dt;
			}
			else if (enemyPosCurrent.x < startPosition.x)
			{
				velocityX += 1.8f*dt;
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
		
	}

	if (!collidingWithGround)
	{
		velocityY -= 0.8*dt;
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
	enemyPosCurrent.y += velocityY;

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
