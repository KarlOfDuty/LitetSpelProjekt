#include "EnemyCrab.h"
#include "Player.h"
#include "Trigger.h"

EnemyCrab::EnemyCrab(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, SoundSystem * sound) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor, sound)
{
	this->acceleration = 5.0f;
	this->moving = true;
	this->oldOriginPoint = enemyStartPos;
	this->startPosition = enemyStartPos;
	this->sound = sound;
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
		if (walkTimer.getElapsedTime().asSeconds() >= 1.4)
		{
			moving = true;
		}
	}

	if (moving)
	{
		if (glm::length(enemyPosCurrent - oldOriginPoint) > 100.0f)
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
		if (abs(collisionNormal.x) > 0.8)
		{
			collisionCounter++;
			movingRight = false;
			movingLeft = false;
			moving = false;
			velocityX = 0;
			oldOriginPoint = enemyPosCurrent;
			walkTimer.restart();
			if (collisionCounter > 10)
			{
				returnToStart = true;
				playerSeen = false;
			}
			timeSinceCollision.restart();
		}
	}

	if (collidedFrom.x == 0)
	{
		collisionTime.restart();
	}

	if (timeSinceCollision.getElapsedTime().asSeconds() >= 2)
	{
		collisionCounter = 0;
	}

	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 300.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	if (playerSeen == true && soundTimer.getElapsedTime().asSeconds() > 6)
	{
		this->sound->playSound("snapyCraby");
		soundTimer.restart();
	}

	//Move
	if (collidingWithGround)
	{
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
				if (glm::length(enemyPosCurrent.x - startPosition.x) > 10.5f)
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
	}

	if (!collidingWithGround)
	{
		velocityY -= 300 * dt;
	}

	if (velocityY > 200)
	{
		velocityY = 200;
	}

	//Maximum falling speed
	if (velocityY < -300)
	{
		velocityY = -300;
	}

	if (velocityX < -5.0) velocityX = -5.0f;
	if (velocityX > 5.0) velocityX = 5.0f;


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
