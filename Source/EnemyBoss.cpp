#include "EnemyBoss.h"

EnemyBoss::EnemyBoss(int HP, Model* model, int damage, glm::vec3 enemyStartPos) :EnemyChar(HP, model, damage, enemyStartPos)
{
	this->acceleration = 0.3f;
	this->originPoint = enemyStartPos;
	this->attackNow = true;
	this->phase1 = true;
	this->phase2 = false;
	this->phase3 = false;
	this->chargeCounter = 0;
}

EnemyBoss::~EnemyBoss()
{

}

void EnemyBoss::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBoss::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels)
{
	if (glm::length(enemyPosCurrent - playerPos) < 30.0f)
	{
		groundCheck();

		if (phase1)
		{
			if (!attackNow)
			{
				if (walkTimer.getElapsedTime().asSeconds() >= 1.5)
				{
					attackNow = true;
				}
			}

			if (attackNow)
			{
				if (glm::length(enemyPosCurrent - originPoint) > 8.0f)//replace with when collision happens
				{
					originPoint = enemyPosCurrent;
					attackNow = false;
					movingRight = false;
					movingLeft = false;
					velocityX = 0;
					walkTimer.restart();
					chargeCounter = chargeCounter + 1;
					if (chargeCounter >= 3)
					{
						dazeTimer.restart();
					}
				}
			}

			if (playerSeen)
			{
				if (isOnGround)
				{
					//Charge
					if (chargeCounter < 3)
					{
						if (attackNow)
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
						}
					}

					if (chargeCounter >= 3)
					{
						if (dazeTimer.getElapsedTime().asSeconds() >= 5)
						{
							chargeCounter = 0;
						}
					}
				}
			}

			if (velocityX < -0.4) velocityX = -0.4f;
			if (velocityX > 0.4) velocityX = 0.4f;

			//Apply velocity
			enemyPosCurrent.x += velocityX;
			enemyPosCurrent.y += velocityY*dt;
		}

		if (phase2)
		{

		}

		if (phase3)
		{

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

		//Handle collision detection with ground
		if (enemyPosCurrent.y <= 0) {
			velocityY = 0;
			enemyPosCurrent.y = 0;
			isOnGround = true;
		}

		setEnemyPos(enemyPosCurrent);
	}
}
