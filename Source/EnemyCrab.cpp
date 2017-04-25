#include "EnemyCrab.h"

EnemyCrab::EnemyCrab(int HP, Model model, int damage, glm::vec3 enemyPos) :EnemyChar(HP, model, damage, enemyPos)
{
	acceleration = 0.2f;
	this->checkPointThis.x = enemyPos.x;
}

EnemyCrab::~EnemyCrab()
{

}

void EnemyCrab::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos)
{

}

void EnemyCrab::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos)
{
	groundCheck();


	if (fabs(enemyPosCurrent.x) > checkPoint.x+5)
	{
		test = true;
		std::cout << "go right" << std::endl;
		std::cout << enemyPosCurrent.x << std::endl;
		std::cout << checkPoint.x+5 << std::endl;
	}
	if (fabs(enemyPosCurrent.x) < checkPoint.x-5)
	{
		test = false;
		std::cout << "go left" << std::endl;
		std::cout << enemyPosCurrent.x << std::endl;
		std::cout << checkPoint.x-5 << std::endl;
	}

	if (walkTimer.getElapsedTime().asSeconds() >= 3.0)
	{
		attackNow = true;
		originPoint = enemyPosCurrent;
		walkTimer.restart();
	}

	if (glm::length(enemyPosCurrent - originPoint) > 4.0f)
	{
		attackNow = false;
		movingRight = false;
		movingLeft = false;
		velocityX = 0;
	}

	if (isOnGround)
	{
		//Move
		if (attackNow)
		{
			if (glm::length(enemyPosCurrent - playerPos) < 5.0f || playerSeen)
			{
				if (movingLeft == false)
				{
					if (enemyPosCurrent.x >= playerPos.x)
					{
						movingRight = true;
					}
				}
				if (movingRight == false)
				{
					if (enemyPosCurrent.x <= playerPos.x)
					{
						movingLeft = true;
					}
				}
				if (movingRight == true)
				{
					velocityX = velocityX - acceleration * dt;
				}
				else if (movingLeft == true)
				{
					velocityX = velocityX + acceleration * dt;
				}
				playerSeen = true;
			}
			else
			{
				//Patrol
				if (!test)
				{
					velocityX = velocityX - acceleration * dt;
				}
				else if (test)
				{
					velocityX = velocityX + acceleration * dt;
				}
			}
		}
	}

	if (!isOnGround)
	{
		velocityY -= 30 * dt;
	}

	if (velocityY > 10)
	{
		velocityY = 10;
	}

	if (velocityY < -10)
	{
		velocityY = -10;
	}

	if (velocityX < -0.3) velocityX = -0.3f;
	if (velocityX > 0.3) velocityX = 0.3f;


	//Apply velocity
	enemyPosCurrent.x += velocityX;
	enemyPosCurrent.y += velocityY*dt;

	//Handle collision detection with ground
	if (enemyPosCurrent.y <= 0) {
		velocityY = 0;
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}

	setEnemyPos(enemyPosCurrent);
}
