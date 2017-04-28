#include "EnemyFireFly.h"

EnemyFireFly::EnemyFireFly(int HP, Model* model, int damage, glm::vec3 enemyPos) :EnemyChar(HP, model, damage, enemyPos)
{

}

EnemyFireFly::~EnemyFireFly()
{

}

void EnemyFireFly::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos)
{

}

void EnemyFireFly::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();

	//Move
	if (glm::length(enemyPos - playerPos) < 10.0f || playerSeen)
	{
		if (enemyPos.x > playerPos.x)
		{
			velocityX -= 1.8f*dt;
		}
		else if (enemyPos.x < playerPos.x)
		{
			velocityX += 1.8f*dt;
		}
		if (enemyPos.y > playerPos.y)
		{
			velocityY -= 1.8f*dt;
		}
		else if (enemyPos.y < playerPos.y)
		{
			velocityY += 1.8f*dt;
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
	enemyPos.x += velocityX;
	velocityX = 0;
	enemyPos.y += velocityY;
	velocityY = 0;

	//Handle collision detection with ground
	if (enemyPos.y <= 0) {
		enemyPos.y = 0;
		isOnGround = true;
	}

	setPos(enemyPos);
}
