#include "EnemySlime.h"

EnemySlime::EnemySlime(int HP, Model* model, int damage, glm::vec3 enemyStartPos) :EnemyChar(HP, model, damage, enemyStartPos)
{
	
}

EnemySlime::~EnemySlime()
{

}

void EnemySlime::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemySlime::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels)
{
	groundCheck();

	//Patrol check 
	if (enemyPosCurrent.x < checkPoint.x-2)
	{
		checkPointReached = true;
		
	}
	else if (enemyPosCurrent.x > checkPoint.x+2)
	{
		checkPointReached = false;

	}

	//Move
	if (glm::length(enemyPosCurrent - playerPos) < 5.0f || playerSeen)
	{
		if (enemyPosCurrent.x > playerPos.x)
		{
			velocityX -= 1.0f*dt;
		}
		else
		{
			velocityX += 1.0f*dt;
		}
		playerSeen = true;
	}
	else
	{
		//Patrol
		if(checkPointReached == false)
		{
			velocityX -= 1.0f*dt;
		}
		else if (checkPointReached == true)
		{
			velocityX += 1.0f*dt;
		}
	}

	if (isOnGround)
	{
		if (fabs(enemyPosCurrent.x - playerPos.x) < 0.1)
		{
			velocityY = 10;
		}
	}

	if (!isOnGround)
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

	/*int index = -1;
	float minDist = 1000;
	for (int i = 0; i < allModels.size(); i++)
	{
		float distance = glm::length(enemyPosCurrent - glm::vec3(allModels[i]->getModelMatrix()[3]));
		if (minDist > distance)
		{
			minDist = distance;
			index = i;
		}
	}
	if (index != -1)
	{
		std::vector<glm::vec2> enemyPoints = this->getModel().getPoints();
		std::vector<glm::vec2> objectPoints = allModels[index]->getPoints();
		glm::vec2 mtv;
		if (collision::fixCollision(enemyPoints, objectPoints, mtv))
		{
			enemyPosCurrent.x += mtv.x;
			enemyPosCurrent.y += mtv.y;
		}
	}*/

	//Handle collision detection with ground
	if (enemyPosCurrent.y <= 0) {
		velocityY = 0;
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}

	setEnemyPos(enemyPosCurrent);
}

