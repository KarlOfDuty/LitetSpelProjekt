#include "EnemyBoss.h"
#include "Player.h"
#include "Trigger.h"

EnemyBoss::EnemyBoss(int health, Model* model, int damage, glm::vec3 enemyStartPos, glm::vec3 scaleFactor) :Enemy(health, model, damage, enemyStartPos, scaleFactor)
{
	this->acceleration = 0.4f;
	this->originPoint = enemyStartPos;
	this->attacking = true;
	this->phase1 = true;
	this->phase2 = false;
	this->phase3 = false;
	this->chargeCounter = 0;
	this->fightStart = false;
	bossImmunity = true;
	
	boxModel = new Model("models/cube/cube.obj");
}

EnemyBoss::~EnemyBoss()
{

}

void EnemyBoss::weakPoints(std::vector<GameObject*> allProjectiles)
{
	std::vector<glm::vec2> weakPoint1 = { glm::vec2(getPos().x + 2,getPos().y + -1), glm::vec2(getPos().x + 2,getPos().y + 0), glm::vec2(getPos().x + 3,getPos().y + -1), glm::vec2(getPos().x + 3,getPos().y + 0) };
	TriggerSettings bossSettings;
	bossSettings.onEnter = true;
	weakPointsArr.push_back(new Trigger(weakPoint1, bossSettings, allProjectiles, this, "applyDamage"));
}

void EnemyBoss::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBoss::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
{
	if (collidedFrom.y > 0)
	{
		collidingWithGround = true;
	}
	else if (collidedFrom.y <= 0)
	{
		collidingWithGround = false;
	}

	if (!fightStart)
	{
		std::vector<GameObject*> playerProjectiles;
		for(int i = 0; i < player->getProjectiles().size(); i++)
		{
			playerProjectiles.push_back(player->getProjectileAt(i));
		}
		weakPoints(playerProjectiles);
		fightStart = true;
	}

	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 1.0f)
	{
		playerSeen = true;
	}

		groundCheck();



		if (collides)
		{
			if (collisionTime.getElapsedTime().asSeconds() >= 5)
			{
				returnToStart = true;
				playerSeen = false;
			}
		}

		if (collidedFrom.x == 0)
		{
			collisionTime.restart();
		}

		if (phase1)
		{
			if (!attacking)
			{
				if (walkTimer.getElapsedTime().asSeconds() >= 1.0)
				{
					attacking = true;
				}
			}

			if (attacking)
			{
				/*if (collidedFrom.x != 0)
				{*/
					if (glm::length(enemyPosCurrent - originPoint) > 15.0f)//replace with when collision happens
					{
						originPoint = enemyPosCurrent;
						attacking = false;
						movingRight = false;
						movingLeft = false;
						velocityX = 0;
						walkTimer.restart();
						chargeCounter = chargeCounter + 1;
					}
				/*}*/
			}

			if (playerSeen)
			{
				if (collidingWithGround)
				{
					//Charge
					if (chargeCounter < 3)
					{
						if (!attacking)
						{
							if (enemyPosCurrent.x >= player->getPos().x)
							{
								rotateLeft = false;
							}
							if (enemyPosCurrent.x <= player->getPos().x)
							{
								rotateLeft = true;
							}
						}

						if (attacking)
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
						}
						dazeTimer.restart();
					}

					if (chargeCounter >= 3)
					{
						bossImmunity = false;
						if (dazeTimer.getElapsedTime().asSeconds() >= 5)
						{
							if (enemyPosCurrent.x >= player->getPos().x)
							{
								rotateLeft = false;
							}
							if (enemyPosCurrent.x <= player->getPos().x)
							{
								rotateLeft = true;
							}
							bossImmunity = true;
							chargeCounter = 0;
						}
					}
				}
			}

			if (velocityX < -0.5) velocityX = -0.5f;
			if (velocityX > 0.5) velocityX = 0.5f;

		}

		if (phase2)
		{

		}

		if (phase3)
		{

		}


		if (!collidingWithGround)
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

			//Apply velocity
		if (phase1)
		{
			enemyPosCurrent.x += velocityX;
			enemyPosCurrent.y += velocityY*dt;
		}

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

		for (int i = 0; i < weakPointsArr.size(); i++)
		{
			weakPointsArr[i]->move(glm::vec2(velocityX, velocityY*dt));
		}

		for (int i = 0; i < weakPointsArr.size(); i++)
		{
			weakPointsArr[i]->update(dt);
		}

		for (int i = 0; i < 4; i++)
		{
			if (debugWeakPointsBox.size() <= i)
			{
				Model* temp;
				temp = boxModel;
				std::vector<glm::vec2> points = weakPointsArr[0]->getPoints();
				glm::mat4 shitmat = {
					0.2, 0, 0, 0,
					0, 0.2, 0, 0,
					0, 0, 0.2, 0,
					points[i].x, points[i].y, 0, 1
				};

				debugWeakPointsBox.push_back(new Model(boxModel, shitmat));
			}
			else
			{
				std::vector<glm::vec2> points = weakPointsArr[0]->getPoints();
				glm::mat4 shitmat = {
					0.2, 0, 0, 0,
					0, 0.2, 0, 0,
					0, 0, 0.2, 0,
					points[i].x, points[i].y, 0, 1
				};
				debugWeakPointsBox[i]->setModelMatrix(shitmat);
			}
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

std::vector<Model*> EnemyBoss::getDebugModels()
{
	return debugWeakPointsBox;
}
