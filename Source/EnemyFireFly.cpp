#include "EnemyFireFly.h"

EnemyFireFly::EnemyFireFly(int HP, Model model, int damage, glm::vec3 enemyPosCurrent) :EnemyChar(HP, model, damage, enemyPosCurrent)
{
	this->attackRange = 9;
}

EnemyFireFly::~EnemyFireFly()
{

}

void EnemyFireFly::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyFireFly::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	//Move
	if (glm::length(enemyPosCurrent - playerPos) < 10.0f || playerSeen)
	{
		if (enemyPosCurrent.x > playerPos.x+attackRange)
		{
			velocityX -= 1.5f*dt;
		}
		else if (enemyPosCurrent.x < playerPos.x-attackRange)
		{
			velocityX += 1.5f*dt;
		}
		if (enemyPosCurrent.y > playerPos.y)
		{
			velocityY -= 1.5f*dt;
		}
		else if (enemyPosCurrent.y < playerPos.y)
		{
			velocityY += 1.5f*dt;
		}
		playerSeen = true;
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
}