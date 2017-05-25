#include "EnemySkeleton.h"
#include "Player.h"
#include "Trigger.h"

EnemySkeleton::EnemySkeleton(int health, Model* model, int damage, int immunityTime, bool patrol, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor)
{
	this->attack = true;
	this->acceleration = 0.8f;
	this->patrol = patrol;
	this->startPosition = enemyStartPos;
	this->attackRange = 3.0f;
	this->allProjectiles = allProjectiles;
	this->checkPointGiven = false;
	box = new Model("models/cube/cubeGreen.obj");
}

EnemySkeleton::~EnemySkeleton()
{

}

void EnemySkeleton::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{
	if (waitBeforeAttack.getElapsedTime().asSeconds() >= 0.1)
	{
		if (attackCooldown.getElapsedTime().asSeconds() >= 1)
		{
			glm::vec3 scale(0.0f, 2.0f, 1.0f);
			glm::vec2 direction = (getPos().x >= playerPos.x) ? glm::vec2(-1, 0) : glm::vec2(1, 0);
			Projectile* temp = new Projectile;
			temp->enemyMelee(box, getPos(), direction, 10.0f, scale);
			allProjectiles->push_back(temp);
			attack = false;
			jumpDelay.restart();
			attackCooldown.restart();
			waitTimer.restart();
		}
	}
}

void EnemySkeleton::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*>& allModels, Player* player)
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

	if (collidingWithGround)
	{
		if (!attack)
		{
			if (!checkPointGiven)
			{
				if (enemyPosCurrent.x > player->getPos().x)
				{
					Dodgecheckpoint.x = player->getPos().x + 10;
					Dodgecheckpoint.y = enemyPosCurrent.y;
					Dodgecheckpoint.z = enemyPosCurrent.z;
					dodgeLeft = false;
					checkPointGiven = true;
				}
				else if (enemyPosCurrent.x < player->getPos().x)
				{
					Dodgecheckpoint.x = player->getPos().x - 10;
					Dodgecheckpoint.y = enemyPosCurrent.y;
					Dodgecheckpoint.z = enemyPosCurrent.z;
					dodgeLeft = true;
					checkPointGiven = true;
				}
			}
		}

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
	}

	if (collides)
	{
		if (collidedFrom.x != 0 && collidedFrom.y > 0)
		{
			velocityY = 15;
		}

		if (collidedFrom.x != 0)
		{
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
	if (glm::length(enemyPosCurrent - player->getPos()) < 10.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

		if (!returnToStart)
		{
			//Move
			if (playerSeen)
			{
				if (attack)
				{
					if (enemyPosCurrent.x > player->getPos().x)
					{
						rotateLeft = false;
					}
					else if (enemyPosCurrent.x < player->getPos().x)
					{
						rotateLeft = true;
					}
					if (glm::length(enemyPosCurrent.x - player->getPos().x) > attackRange)
					{
						if (enemyPosCurrent.x > player->getPos().x + attackRange)
						{
							velocityX -= 3.8f*dt;
						}
						else if (enemyPosCurrent.x < player->getPos().x - attackRange)
						{
							velocityX += 3.8f*dt;
						}
						waitBeforeAttack.restart();
					}
					else
					{
						this->attackPlayer(dt, player->getPos(), enemyPosCurrent);
					}
				}
				else
				{
					if (waitTimer.getElapsedTime().asSeconds() >= 0.15)
					{
						if (!dodgeLeft)
						{
							if (!jumped)
							{
								if (collidingWithGround)
								{
									if (jumpDelay.getElapsedTime().asSeconds() >= 0.2)
									{
										velocityY = 7;
										jumped = true;
									}
								}
							}
							velocityX = velocityX + acceleration * dt;
						}
						else
						{
							if (!jumped)
							{
								if (collidingWithGround)
								{
									if (jumpDelay.getElapsedTime().asSeconds() >= 0.2)
									{
										velocityY = 7;
										jumped = true;
									}
								}
							}
							velocityX = velocityX - acceleration * dt;
						}

						if (fabs(enemyPosCurrent.x - Dodgecheckpoint.x) < 0.5f)
						{
							jumped = false;
							velocityX = 0;
							attack = true;
							dodgeLeft = false;
							checkPointGiven = false;
						}
					}
				}
			}
			else
			{
				//Patrol
				if (patrol)
				{
					if (enemyPosCurrent.x >= checkPoint.x + 3)
					{
						rotateLeft = false;
					}
					if (enemyPosCurrent.x <= checkPoint.x - 3)
					{
						rotateLeft = true;
					}
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
		else
		{
			if (enemyPosCurrent.x >= startPosition.x)
			{
				rotateLeft = false;
			}
			if (enemyPosCurrent.x <= startPosition.x)
			{
				rotateLeft = true;
			}
			if (glm::length(enemyPosCurrent.x - startPosition.x) > 0.5f)
			{
				if (enemyPosCurrent.x > startPosition.x)
				{
					velocityX -= 1.8f*dt;
				}
				else if (enemyPosCurrent.x < startPosition.x)
				{
					velocityX += 1.8f*dt;
				}
			}
			else
			{
				returnToStart = false;
				playerSeen = false;
			}
		}

	if (!collidingWithGround)
	{
		velocityY -= 30 * dt;
	}

	if (velocityY > 15)
	{
		velocityY = 15;
	}

	//Maximum falling speed
	if (velocityY < -30)
	{
		velocityY = -30;
	}


	if (!attack)
	{
		if (velocityX < -1.0) velocityX = -1.0f;
		if (velocityX > 1.0) velocityX = 1.0f;
	}

	//Apply velocity
	enemyPosCurrent.x += velocityX;
	if (attack)
	{
		velocityX = 0;
	}
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
