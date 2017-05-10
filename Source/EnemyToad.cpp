#include "EnemyToad.h"
#include "Player.h"

EnemyToad::EnemyToad(int health, Model* model, int damage, glm::vec3 enemyStartPos) :Enemy(health, model, damage, enemyStartPos)
{
	this->startPosition = enemyStartPos;
	this->returnToStart = false;
}

EnemyToad::~EnemyToad()
{

}

void EnemyToad::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyToad::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
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
			if (collidedFrom.x != 0)
			{
				collisionCounterToad++;
				if (collisionCounterToad > 200)
				{
					returnToStart = true;
					playerSeen = false;
				}
				timeSinceCollision.restart();
			}
		}

		if (timeSinceCollision.getElapsedTime().asSeconds() >= 2)
		{
			collisionCounterToad = 0;
		}

		//Detect player
		if (glm::length(enemyPosCurrent - player->getPos()) < 5.0f)
		{
			playerSeen = true;
			returnToStart = false;
		}

		if (!returnToStart)
		{
			if (collidingWithGround)
			{
				//Jump
				if (playerSeen)
				{
					if (jumpTimer.getElapsedTime().asSeconds() >= 1.6)
					{
						if (collidingWithGround)
						{
							velocityY = 15;
						}
						jumpTimer.restart();
					}

					playerSeen = true;
				}
				movingRight = false;
				movingLeft = false;
			}

			//Move in air only
			if (!collidingWithGround)
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
					velocityX -= 3.0f*dt;
				}
				else if (movingLeft == true)
				{
					velocityX += 3.0f*dt;
				}
				velocityY -= 30 * dt;
			}
		}
		else
		{
			if (collidingWithGround)
			{
				//Jump
				if (glm::length(enemyPosCurrent.x - startPosition.x) > 0.5f)
				{
					if (jumpTimer.getElapsedTime().asSeconds() >= 1.4)
					{
						if (collidingWithGround)
						{
							velocityY = 15;
						}
						jumpTimer.restart();
					}
				}
				else
				{
					returnToStart = false;
					playerSeen = false;
				}
				movingRight = false;
				movingLeft = false;
			}

			//Move in air only
			if (!collidingWithGround)
			{
				if (movingLeft == false)
				{
					if (enemyPosCurrent.x >= startPosition.x)
					{
						movingRight = true;
					}
				}
				if (movingRight == false)
				{
					if (enemyPosCurrent.x <= startPosition.x)
					{
						movingLeft = true;
					}
				}
				if (movingRight == true)
				{
					velocityX -= 3.0f*dt;
				}
				else if (movingLeft == true)
				{
					velocityX += 3.0f*dt;
				}
				velocityY -= 30 * dt;
			}
		}

		if (velocityY < -30)
		{
			velocityY = -30;
		}
		if (velocityX > 3)
		{
			velocityX = 3;
		}
		if (velocityX < -3)
		{
			velocityX = -3;
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

