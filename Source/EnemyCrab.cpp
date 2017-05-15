#include "EnemyCrab.h"
#include "Player.h"

EnemyCrab::EnemyCrab(int health, Model* model, int damage, glm::vec3 enemyStartPos, glm::vec3 scaleFactor) :Enemy(health, model, damage, enemyStartPos, scaleFactor)
{
	this->acceleration = 0.2f;
	this->moving = true;
	this->oldOriginPoint = enemyStartPos;
	this->startPosition = enemyStartPos;
}

EnemyCrab::~EnemyCrab()
{

}

void EnemyCrab::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyCrab::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
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

	if (enemyPosCurrent.x < checkPoint.x-3)
	{
		checkPointReached = true;
	}
	if (enemyPosCurrent.x > checkPoint.x+3)
	{
		checkPointReached = false;
	}

	if (!moving)
	{
		if (walkTimer.getElapsedTime().asSeconds() >= 1.5)
		{
			moving = true;
		}
	}

	if (moving)
	{
		if (glm::length(enemyPosCurrent - oldOriginPoint) > 4.0f)
		{
			oldOriginPoint = enemyPosCurrent;
			moving = false;
			movingRight = false;
			movingLeft = false;
			velocityX = 0;
			walkTimer.restart();
		}
	}

	if (collides)
	{
		if (collidedFrom.x != 0)
		{
			movingRight = false;
			movingLeft = false;
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
	if (glm::length(enemyPosCurrent - player->getPos()) < 5.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	//Move
	if (moving)
	{
		if (!returnToStart)
		{
			if (playerSeen)
			{
				if (!movingLeft)
				{
					if (enemyPosCurrent.x >= player->getPos().x)
					{
						movingRight = true;
					}
				}
				if (!movingRight)
				{
					if (enemyPosCurrent.x <= player->getPos().x)
					{
						movingLeft = true;
					}
				}
				if (movingRight)
				{
					velocityX = velocityX - acceleration * dt;
				}
				else if (movingLeft)
				{
					velocityX = velocityX + acceleration * dt;
				}
				playerSeen = true;
			}
			else
			{
				//Patrol
				if (!movingLeft)
				{
					if ((!checkPointReached))
					{
						movingRight = true;
					}
				}
				if (!movingRight)
				{
					if (checkPointReached)
					{
						movingLeft = true;
					}
				}
				if (movingRight)
				{
					velocityX = velocityX - acceleration * dt;
				}
				else if (movingLeft)
				{
					velocityX = velocityX + acceleration * dt;
				}
			}
		}
		else
		{
			if (glm::length(enemyPosCurrent.x - startPosition.x) > 0.5f)
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
					velocityX = velocityX - acceleration * dt;
				}
				else if (movingLeft == true)
				{
					velocityX = velocityX + acceleration * dt;
				}
			}
			else
			{
				returnToStart = false;
				playerSeen = false;
			}
		}
	}
	

	if (collidingWithGround)
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

	//Maximum falling speed
	if (velocityY < -30)
	{
		velocityY = -30;
	}

	if (velocityX < -0.3) velocityX = -0.3f;
	if (velocityX > 0.3) velocityX = 0.3f;


	//Apply velocity
	enemyPosCurrent.x += velocityX;
	enemyPosCurrent.y += velocityY*dt;

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
	collides = collision(allModels);
	collisionWithPlayer(player);
}
