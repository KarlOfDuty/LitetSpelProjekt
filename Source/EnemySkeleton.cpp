#include "EnemySkeleton.h"

EnemySkeleton::EnemySkeleton(int HP, Model* model, int damage, bool patrol, glm::vec3 enemyStartPos) :EnemyChar(HP, model, damage, enemyStartPos)
{
	this->patrol = patrol;
	std::srand(time(0));
}

EnemySkeleton::~EnemySkeleton()
{

}

void EnemySkeleton::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{
	//randomize doValue, if above 3 attack, if 1/2/3 block
	doValue = rand() % 10 + 1;
	if (doValue >= 4)
	{
		//attackPlayer
	}
	else if (doValue <= 3)
	{
		//block, or dodge
	}
}

void EnemySkeleton::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*>& allModels)
{
	groundCheck();

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

	if (isOnGround)
	{
		//Move
		if (glm::length(enemyPosCurrent - playerPos) < 5.0f || playerSeen)
		{
			if (enemyPosCurrent.x > playerPos.x)
			{
				velocityX -= 3.8f*dt;
			}
			else
			{
				velocityX += 3.8f*dt;
			}
			playerSeen = true;
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

	if (!isOnGround)
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
	if (enemyPosCurrent.y <= 0) {
		velocityY = 0;
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}

	setPos(enemyPosCurrent);
	checkCollision(allModels);
}
