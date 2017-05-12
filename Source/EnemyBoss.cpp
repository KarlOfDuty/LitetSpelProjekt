#include "EnemyBoss.h"
#include "Player.h"

EnemyBoss::EnemyBoss(int health, Model* model, int damage, glm::vec3 enemyStartPos, glm::vec3 scaleFactor) :Enemy(health, model, damage, enemyStartPos, scaleFactor)
{
	this->acceleration = 0.3f;
	this->originPoint = enemyStartPos;
	this->attacking = true;
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

void EnemyBoss::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
{
	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 30.0f)
	{
		groundCheck();

		if (phase1)
		{
			if (!attacking)
			{
				if (walkTimer.getElapsedTime().asSeconds() >= 1.5)
				{
					attacking = true;
				}
			}

			if (attacking)
			{
				if (glm::length(enemyPosCurrent - originPoint) > 8.0f)//replace with when collision happens
				{
					originPoint = enemyPosCurrent;
					attacking = false;
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
					if (enemyPosCurrent.x >= player->getPos().x)
					{
						rotateLeft = false;
					}
					if (enemyPosCurrent.x <= player->getPos().x)
					{
						rotateLeft = true;
					}
					//Charge
					if (chargeCounter < 3)
					{
						if (attacking)
						{
							if (movingLeft == false)
							{
								if (enemyPosCurrent.x >= player->getPos().x)
								{
									movingRight = true;
								}
							}
							if (movingRight == false)
							{
								if (enemyPosCurrent.x <= player->getPos().x)
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
		collision(allModels);
		if (rotateLeft == false)
		{
			rotateModel(-90.0f);
		}

		if (rotateLeft == true)
		{
			rotateModel(90.0f);
		}
		collisionWithPlayer(player);
	}
}
