#include "EnemyFireFly.h"
#include "Player.h"
#include "Trigger.h"

EnemyFireFly::EnemyFireFly(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor)
{
	this->attackRange = 10;
	this->startPosition = enemyStartPos;
	this->allProjectiles = allProjectiles;
}

EnemyFireFly::~EnemyFireFly()
{

}

void EnemyFireFly::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{
	if (attackCooldown.getElapsedTime().asSeconds() >= 2)
	{
		int activeArrows = 0;
		for (int i = 0; i < allProjectiles->size(); i++)
		{
			if (allProjectiles->at(i)->isInUse())
				activeArrows++;
		}

		glm::vec2 direction = (getPos().x >= playerPos.x) ? glm::vec2(-1, 0) : glm::vec2(1, 0);
		if (activeArrows < allProjectiles->size())
		{
			if (activeArrows < allProjectiles->size())
			{
				for (int i = 0; i < allProjectiles->size(); i++)
				{
					if (!allProjectiles->at(i)->isInUse())
					{
						allProjectiles->at(i)->shoot(getModel(), getPos(), direction, glm::vec2(), 10.f, glm::vec3(0.2, 0.2, 0.2),false,true);
						i = (int)allProjectiles->size();
					}
				}
			}
		}
		else
		{
			Projectile* temp = new Projectile;
			temp->shoot(getModel(), getPos(), direction, glm::vec2(), 10.f, glm::vec3(0.2, 0.2, 0.2),false,true);
			allProjectiles->push_back(temp);
		}
		attackCooldown.restart();
	}
}

void EnemyFireFly::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
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
		velocityY += 3.0f*dt;

		if (collisionTime.getElapsedTime().asSeconds() >= 5)
		{
			returnToStart = true;
			playerSeen = false;
		}
	}
	else
	{
		collisionTime.restart();
	}

	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 15.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	//Move
	if (!returnToStart)
	{
		if (playerSeen)
		{
			if (enemyPosCurrent.x > player->getPos().x + attackRange)
			{
				velocityX -= 1.5f*dt;
			}
			else if (enemyPosCurrent.x < player->getPos().x - attackRange)
			{
				velocityX += 1.5f*dt;
			}
			if (enemyPosCurrent.y > player->getPos().y + 2)
			{
				velocityY -= 1.5f*dt;
			}
			else if (enemyPosCurrent.y < player->getPos().y + 2)
			{
				velocityY += 1.5f*dt;
			}
			playerSeen = true;
		}
	}
	else
	{
		if (glm::length(enemyPosCurrent - startPosition) > 0.5f)
		{
			if (!collides)
			{
				if (enemyPosCurrent.x > startPosition.x)
				{
					velocityX -= 1.5f*dt;
				}
				else if (enemyPosCurrent.x < startPosition.x)
				{
					velocityX += 1.5f*dt;
				}
				if (enemyPosCurrent.y > startPosition.y)
				{
					velocityY -= 1.5f*dt;
				}
				else if (enemyPosCurrent.y < startPosition.y)
				{
					velocityY += 1.5f*dt;
				}
			}
			else
			{
				velocityY += 3.0f*dt;
			}
		}
		else
		{
			returnToStart = false;
			playerSeen = false;
		}
	}

	if (glm::length(enemyPosCurrent - player->getPos()) < 11.0f && fabs(enemyPosCurrent.y - player->getPos().y) < 3.0f)
	{
		this->attackPlayer(dt, player->getPos(), enemyPosCurrent);
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
