#include "EnemySkeleton.h"
#include "Player.h"
#include "Trigger.h"

EnemySkeleton::EnemySkeleton(int health, Model* model, int damage, int immunityTime, bool patrol, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles, SoundSystem * sound) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor, sound)
{
	this->attack = true;
	this->acceleration = 13.0f;
	this->patrol = patrol;
	this->startPosition = enemyStartPos;
	this->attackRange = 40.0f;
	this->allProjectiles = allProjectiles;
	this->checkPointGiven = false;
	box = new Model("models/cube/cubeGreen.obj");
	this->sound = sound;
}

EnemySkeleton::~EnemySkeleton()
{

}

void EnemySkeleton::attackPlayer(float dt, glm::vec3 playerPos)
{
	if (waitBeforeAttack.getElapsedTime().asSeconds() >= 0.2)
	{
		if (attackCooldown.getElapsedTime().asSeconds() >= 1)
		{
			glm::vec3 scale(0.0, 75.0, 2.0);
			glm::vec2 direction = (getPos().x >= playerPos.x) ? glm::vec2(-1, 0) : glm::vec2(1, 0);
			Projectile* temp = new Projectile;
			temp->enemyMelee(box, getDamage(), pos, direction, 200.0f, scale);
			allProjectiles->push_back(temp);
			attack = false;
			jumpDelay.restart();
			attackCooldown.restart();
			waitTimer.restart();
		}
	}
}

void EnemySkeleton::update(float dt, std::vector<Enemy*> &allSmallBats, std::vector<Model*> &allModels, Player* player)
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
				if (pos.x > player->getPos().x)
				{
					Dodgecheckpoint.x = player->getPos().x + 150;
					Dodgecheckpoint.y = pos.y;
					Dodgecheckpoint.z = pos.z;
					dodgeLeft = false;
					checkPointGiven = true;
				}
				else if (pos.x < player->getPos().x)
				{
					Dodgecheckpoint.x = player->getPos().x - 150;
					Dodgecheckpoint.y = pos.y;
					Dodgecheckpoint.z = pos.z;
					dodgeLeft = true;
					checkPointGiven = true;
				}
			}
		}

		//Patrol check 
		if (patrol)
		{
			if (pos.x < checkPoint.x - 100)
			{
				checkPointReached = true;
			}
			else if (pos.x > checkPoint.x + 100)
			{
				checkPointReached = false;
			}
		}
	}

	if (collides)
	{
		if (attack)
		{
			if (collidedFrom.x != 0 && collidedFrom.y > 0)
			{
				velocityY = 200;
			}
		}


		if (!attack)
		{
			if (collidedFrom.x != 0 && collidedFrom.y > 0)
			{
				jumped = false;
				velocityX = 0;
				attack = true;
				dodgeLeft = false;
				checkPointGiven = false;
			}
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
	if (glm::length(pos - player->getPos()) < 200.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	if (playerSeen == true && soundTimer.getElapsedTime().asSeconds() > 8)
	{
		this->sound->playSound("clickySkeliClacks");
		soundTimer.restart();
	}


	if (!returnToStart)
	{
		//Move
		if (playerSeen)
		{
			if (attack)
			{
				if (pos.x > player->getPos().x)
				{
					rotateLeft = false;
				}
				else if (pos.x < player->getPos().x)
				{
					rotateLeft = true;
				}
				if (glm::length(pos.x - player->getPos().x) > attackRange)
				{
					if (pos.x > player->getPos().x + attackRange)
					{
						velocityX -= 70.0f*dt;
					}
					else if (pos.x < player->getPos().x - attackRange)
					{
						velocityX += 70.0f*dt;
					}
					waitBeforeAttack.restart();
				}
				else
				{
					if (glm::length(pos.y - player->getPos().y) < 70.0f)
					{
						this->attackPlayer(dt, player->getPos());
					}
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
									velocityY = 120;
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
									velocityY = 120;
									jumped = true;
								}
							}
						}
						velocityX = velocityX - acceleration * dt;
					}

					if (fabs(pos.x - Dodgecheckpoint.x) < 10.5f)
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
				if (pos.x >= checkPoint.x + 100)
				{
					rotateLeft = false;
				}
				if (pos.x <= checkPoint.x - 100)
				{
					rotateLeft = true;
				}
				if (checkPointReached == false)
				{
					velocityX -= 50.8f*dt;
				}
				else if (checkPointReached == true)
				{
					velocityX += 50.8f*dt;
				}
			}
		}
	}
	else
	{
		if (pos.x >= startPosition.x)
		{
			rotateLeft = false;
		}
		if (pos.x <= startPosition.x)
		{
			rotateLeft = true;
		}
		if (glm::length(pos.x - startPosition.x) > 10.5f)
		{
			if (pos.x > startPosition.x)
			{
				velocityX -= 50.8f*dt;
			}
			else if (pos.x < startPosition.x)
			{
				velocityX += 50.8f*dt;
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
		velocityY -= 400 * dt;
	}

	if (velocityY > 200)
	{
		velocityY = 200;
	}

	//Maximum falling speed
	if (velocityY < -400)
	{
		velocityY = -400;
	}


	if (!attack)
	{
		if (velocityX < -15.0) velocityX = -15.0f;
		if (velocityX > 15.0) velocityX = 15.0f;
	}

	//Apply velocity
	pos.x += velocityX;
	if (attack)
	{
		velocityX = 0;
	}
	pos.y += velocityY*dt;

	//Handle collision detection with ground
	if (pos.y <= groundPos)
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
