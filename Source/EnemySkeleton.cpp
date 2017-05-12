#include "EnemySkeleton.h"
#include "Player.h"

EnemySkeleton::EnemySkeleton(int health, Model* model, int damage, bool patrol, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles) :Enemy(health, model, damage, enemyStartPos, scaleFactor)
{
	this->attack = true;
	this->acceleration = 0.3f;
	this->patrol = patrol;
	this->startPosition = enemyStartPos;
	this->attackRange = 4.0f;
	this->allProjectiles = allProjectiles;
	box = new Model("models/cube/cubeGreen.obj");
}

EnemySkeleton::~EnemySkeleton()
{

}

void EnemySkeleton::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{
	if (attack)
	{
		if (attackCooldown.getElapsedTime().asSeconds() >= 1)
		{
			glm::vec3 scale(0.0f, 2.0f, 1.0f);
			glm::vec2 direction = (getPos().x >= playerPos.x) ? glm::vec2(-1, 0) : glm::vec2(1, 0);
			Projectile* temp = new Projectile;
			temp->enemyMelee(box, getPos(), direction, 10.0f, scale);
			allProjectiles->push_back(temp);
			attack = false;
			attackCooldown.restart();
		}
	}
	else
	{
		if (dodgeLeft)
		{
			velocityX = velocityX + acceleration * dt;
		}
		else
		{
			velocityX = velocityX - acceleration * dt;
		}

		if (fabs(enemyPosCurrent.x - Dodgecheckpoint.x) < 0.5f)
		{
			attack = true;
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

	if (!attack)
	{
		if (enemyPosCurrent.x > player->getPos().x)
		{
			Dodgecheckpoint.x = enemyPosCurrent.x - 5;
			Dodgecheckpoint.y = enemyPosCurrent.y;
			Dodgecheckpoint.z = enemyPosCurrent.z;
			dodgeLeft = true;
		}
		else if (enemyPosCurrent.x < player->getPos().x)
		{
			Dodgecheckpoint.x = enemyPosCurrent.x + 5;
			Dodgecheckpoint.y = enemyPosCurrent.y;
			Dodgecheckpoint.z = enemyPosCurrent.z;
			dodgeLeft = false;
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

	if (collides)
	{
		if (collidedFrom.x != 0 && collidedFrom.y > 0)
		{
			velocityY = 10;
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
	if (glm::length(enemyPosCurrent - player->getPos()) < 5.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	if (!returnToStart)
	{
		//Move
		if (playerSeen)
		{
				if (enemyPosCurrent.x > player->getPos().x)
				{
					rotateLeft = false;
				}
				else if (enemyPosCurrent.x < player->getPos().x)
				{
					rotateLeft = true;
				}
			if (glm::length(enemyPosCurrent - player->getPos()) > attackRange)
			{
				if (enemyPosCurrent.x > player->getPos().x + attackRange)
				{
					velocityX -= 3.8f*dt;
				}
				else if (enemyPosCurrent.x < player->getPos().x - attackRange)
				{
					velocityX += 3.8f*dt;
				}
			}
			else
			{
				this->attackPlayer(dt, player->getPos(), enemyPosCurrent);
			}
		}
		else
		{
			//Patrol
			if (patrol)
			{
				if (enemyPosCurrent.x >= checkPoint.x)
				{
					rotateLeft = false;
				}
				if (enemyPosCurrent.x <= checkPoint.x)
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

	if (collidingWithGround)
	{
		
	}

	if (!collidingWithGround)
	{
		velocityY -= 0.8*dt;
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
	enemyPosCurrent.y += velocityY;

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
