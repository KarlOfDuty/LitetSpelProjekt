#include "EnemyToad.h"
#include "Player.h"
#include "Trigger.h"

EnemyToad::EnemyToad(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles, SoundSystem * sound) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor, sound)
{
	this->attackRange = 100;
	this->startPosition = enemyStartPos;
	this->returnToStart = false;
	this->allProjectiles = allProjectiles;
	projectileModel = new Model("models/sphere/sphere.obj");
	this->sound = sound;

	projectileModel = new Model("models/sphere/sphereGreen.obj");
}

EnemyToad::~EnemyToad()
{

}

void EnemyToad::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{
	if (attackCooldown.getElapsedTime().asSeconds() >= 2)
	{
		glm::vec2 direction = (getPos().x >= playerPos.x) ? glm::vec2(-0.6, 0.3) : glm::vec2(0.6, 0.3);
		int activeArrows = 0;
		for (int i = 0; i < allProjectiles->size(); i++)
		{
			if (allProjectiles->at(i)->isInUse())
				activeArrows++;
		}
		if (activeArrows < allProjectiles->size())
		{
			if (activeArrows < allProjectiles->size())
			{
				for (int i = 0; i < allProjectiles->size(); i++)
				{
					if (!allProjectiles->at(i)->isInUse())
					{
						allProjectiles->at(i)->shoot(projectileModel, getPos(), direction, glm::vec2(0, 100.f), 200.f, glm::vec3(4.0, 4.0, 4.0),false,true);
						i = (int)allProjectiles->size();
					}
				}
			}
		}
		else
		{
			Projectile* temp = new Projectile;
			temp->shoot(projectileModel, getPos(), direction, glm::vec2(0, 100.f), 200.f, glm::vec3(4.0, 4.0, 4.0),false,true);
			allProjectiles->push_back(temp);
		}
		attackCooldown.restart();
	}
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
		if (glm::length(enemyPosCurrent - player->getPos()) < 200.0f)
		{

			playerSeen = true;
			returnToStart = false;


		}

		if (playerSeen == true && soundTimer.getElapsedTime().asSeconds() > 15 )
		{
			this->sound->playSound("chukelingFrogNoise");
			soundTimer.restart();
		}

		if (!returnToStart)
		{
			if (collidingWithGround)
			{
				if (enemyPosCurrent.x >= player->getPos().x)
				{
					rotateLeft = false;
				}
				if (enemyPosCurrent.x <= player->getPos().x)
				{
					rotateLeft = true;
				}
				if (glm::length(enemyPosCurrent - player->getPos()) > 60.0f + attackRange)
				{
					//Jump
					if (playerSeen)
					{
						if (jumpTimer.getElapsedTime().asSeconds() >= 1.7)
						{
							velocityY = 200;
							jumpTimer.restart();
						}

						playerSeen = true;
					}
					movingRight = false;
					movingLeft = false;
				}
				else
				{
					this->attackPlayer(dt, player->getPos(), enemyPosCurrent);
				}
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
					velocityX -= 60.0f*dt;
				}
				else if (movingLeft == true)
				{
					velocityX += 60.0f*dt;
				}
				velocityY -= 300 * dt;
			}
		}
		else
		{
			if (collidingWithGround)
			{
				if (enemyPosCurrent.x >= startPosition.x)
				{
					rotateLeft = false;
				}
				if (enemyPosCurrent.x <= startPosition.x)
				{
					rotateLeft = true;
				}
				//Jump
				if (glm::length(enemyPosCurrent.x - startPosition.x) > 15.0f)
				{
					if (jumpTimer.getElapsedTime().asSeconds() >= 1.7)
					{
						velocityY = 200;
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
					velocityX -= 60.0f*dt;
				}
				else if (movingLeft == true)
				{
					velocityX += 60.0f*dt;
				}
				velocityY -= 300 * dt;
			}
		}

		if (velocityY < -300)
		{
			velocityY = -300;
		}
		if (velocityX > 60)
		{
			velocityX = 60;
		}
		if (velocityX < -60)
		{
			velocityX = -60;
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

