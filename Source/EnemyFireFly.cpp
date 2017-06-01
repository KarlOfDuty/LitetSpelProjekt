#include "EnemyFireFly.h"
#include "Player.h"
#include "Trigger.h"

EnemyFireFly::EnemyFireFly(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles, SoundSystem * sound) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor, sound)
{
	this->attackRange = 250;
	this->startPosition = enemyStartPos;
	this->allProjectiles = allProjectiles;
	this->sound = sound;

	projectileModel = new Model("models/sphere/sphereFire.obj");
}

EnemyFireFly::~EnemyFireFly()
{

}

void EnemyFireFly::attackPlayer(float dt, glm::vec3 playerPos)
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
						allProjectiles->at(i)->shoot(projectileModel, getDamage(), getPos(), direction, glm::vec2(), 150.f, glm::vec3(2.8, 2.8, 2.8),false,true);
						i = (int)allProjectiles->size();
					}
				}
			}
		}
		else
		{
			Projectile* temp = new Projectile;
			temp->shoot(projectileModel, getDamage(), getPos(), direction, glm::vec2(), 150.f, glm::vec3(2.8, 2.8, 2.8),false,true);
			allProjectiles->push_back(temp);
		}
		attackCooldown.restart();
	}
}

void EnemyFireFly::update(float dt, std::vector<Enemy*> &allSmallBats, std::vector<Model*> &allModels, Player* player)
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
		velocityY += 60.0f*dt;
		if (collidedFrom.y > 0)
		{
			if (attackRange > 0)
			{
				attackRange -= 30;
			}
			timeSinceCollision.restart();
		}

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

	if (timeSinceCollision.getElapsedTime().asSeconds() >= 0.5)
	{
		attackRange = 250;
	}

	//Detect player
	if (glm::length(pos - player->getPos()) < 250.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	if (playerSeen == true && soundTimer.getElapsedTime().asSeconds() > 6)
	{
		this->sound->playSound("wierdBuzzing");
		soundTimer.restart();
	}


	//Move
	if (!returnToStart)
	{
		if (playerSeen)
		{
			if (pos.x > player->getPos().x + attackRange)
			{
				velocityX -= 40.0f*dt;
			}
			else if (pos.x < player->getPos().x - attackRange)
			{
				velocityX += 40.0f*dt;
			}
			if (pos.y > player->getPos().y + 30)
			{
				velocityY -= 40.0f*dt;
			}
			else if (pos.y < player->getPos().y + 30)
			{
				velocityY += 40.0f*dt;
			}
		}
	}
	else
	{
		if (glm::length(pos - startPosition) > 10.5f)
		{
			if (!collides)
			{
				if (pos.x > startPosition.x)
				{
					velocityX -= 40.0f*dt;
				}
				else if (pos.x < startPosition.x)
				{
					velocityX += 40.0f*dt;
				}
				if (pos.y > startPosition.y)
				{
					velocityY -= 40.0f*dt;
				}
				else if (pos.y < startPosition.y)
				{
					velocityY += 40.0f*dt;
				}
			}
		}
		else
		{
			returnToStart = false;
			playerSeen = false;
		}
	}

	if (glm::length(pos - player->getPos()) < 260.0f && fabs(pos.y - player->getPos().y) < 40.0f)
	{
		this->attackPlayer(dt, player->getPos());
	}


	if (isOnGround)
	{

	}

	if (!isOnGround)
	{

	}

	//Apply velocity
	pos.x += velocityX;
	velocityX = 0;
	pos.y += velocityY;
	velocityY = 0;

	//Handle collision detection with ground
	if (pos.y <= groundPos && !isOnGround)
	{
		if (velocityY < 0)
		{
			pos.y = groundPos;
			velocityY = 0;
		}
		isOnGround = true;
	}
	setPos(pos);
	collides = collision(allModels);
	collisionWithPlayer(player);
}
