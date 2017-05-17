#include "EnemyBoss.h"
#include "Player.h"
#include "Trigger.h"

EnemyBoss::EnemyBoss(int health, Model* model, int damage, glm::vec3 enemyStartPos, glm::vec3 scaleFactor) :Enemy(health, model, damage, enemyStartPos, scaleFactor)
{
	this->acceleration = 0.4f;
	this->originPoint = enemyStartPos;
	this->attacking = true;
	this->phase = 1;
	this->chargeCounter = 0;
	this->createTrigger = true;
	bossImmunity = true;
	
	boxModel = new Model("models/cube/cube.obj");

	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

EnemyBoss::~EnemyBoss()
{

}

void EnemyBoss::weakPoints(std::vector<GameObject*> allProjectiles, std::string command, int amountOfTimes)
{
	if (!weakPointsArr.empty())
	{
		this->weakPointsArr.clear();
	}
	TriggerSettings bossSettings;
	bossSettings.onEnter = true;
	bossSettings.numberOfActivationsAllowed = amountOfTimes;
	weakPointsArr.push_back(new Trigger(weakPoint, bossSettings, allProjectiles, this, command));
}

void EnemyBoss::editWeakPoint(float xValue, float yValue, Player* player)
{
	if (rotateLeft)
	{
		corners[0] = glm::vec4(getPos().x, getPos().y, getPos().z, 0.0) + glm::vec4(-xValue, yValue+0.5f, 0.0f, 1.0f);
		corners[1] = glm::vec4(getPos().x, getPos().y, getPos().z, 0.0) + glm::vec4(-xValue, yValue, 0.0f, 1.0f);
		corners[2] = glm::vec4(getPos().x, getPos().y, getPos().z, 0.0) + glm::vec4(-xValue+0.5f, yValue+0.5f, 0.0f, 1.0f);
		corners[3] = glm::vec4(getPos().x, getPos().y, getPos().z, 0.0) + glm::vec4(-xValue+0.5f, yValue, 0.0f, 1.0f);
	}
	if (!rotateLeft)
	{
		corners[0] = glm::vec4(getPos().x, getPos().y, getPos().z, 0.0) + glm::vec4(xValue, yValue+0.5f, 0.0f, 1.0f);
		corners[1] = glm::vec4(getPos().x, getPos().y, getPos().z, 0.0) + glm::vec4(xValue, yValue, 0.0f, 1.0f);
		corners[2] = glm::vec4(getPos().x, getPos().y, getPos().z, 0.0) + glm::vec4(xValue+0.5f, yValue+0.5f, 0.0f, 1.0f);
		corners[3] = glm::vec4(getPos().x, getPos().y, getPos().z, 0.0) + glm::vec4(xValue+0.5f, yValue, 0.0f, 1.0f);
	}

	weakPoint = { glm::vec2(corners[0].x, corners[0].y),
		glm::vec2(corners[1].x, corners[1].y),
		glm::vec2(corners[2].x, corners[2].y),
		glm::vec2(corners[3].x, corners[3].y) };

	playerProjectiles.clear();
	for (int i = 0; i < player->getProjectiles().size(); i++)
	{
		if (player->getProjectileAt(i)->isInUse())
		{
			playerProjectiles.push_back(player->getProjectileAt(i));
		}
	}

	if (!weakPointsArr.empty())
	{
		for (int i = 0; i < weakPointsArr.size(); i++)
		{
			weakPointsArr[i]->setActivators(playerProjectiles);
		}
	}
}

void EnemyBoss::setPhase(int phase)
{
	this->phase = phase;
}

void EnemyBoss::setCreateTrigger(bool createTrigger)
{
	this->createTrigger = createTrigger;
}

void EnemyBoss::setChargeCounter(int amountOfCharges)
{
	this->chargeCounter = amountOfCharges;
}

void EnemyBoss::setRotate(Player* player)
{
	if (getPos().x >= player->getPos().x)
	{
		rotateLeft = false;
	}
	if (getPos().x <= player->getPos().x)
	{
		rotateLeft = true;
	}
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

	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 10.0f)
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

		if (phase == 1)
		{
			editWeakPoint(4.0f, -2.0f, player);

			if (!playerProjectiles.empty())
			{
				if (createTrigger)
				{
					weakPoints(playerProjectiles, "phase1", 4);
					createTrigger = false;
				}
			}

			if (!attacking)
			{
				if (walkTimer.getElapsedTime().asSeconds() >= 1.0)
				{
					attacking = true;
				}
			}

			if (attacking)
			{
				if (collidedFrom.x != 0)
				{
					//if (glm::length(enemyPosCurrent - originPoint) > 15.0f)//replace with when collision happens
					//{
						originPoint = enemyPosCurrent;
						attacking = false;
						movingRight = false;
						movingLeft = false;
						velocityX = 0;
						walkTimer.restart();
						chargeCounter = chargeCounter + 1;
					/*}*/
				}
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
							setRotate(player);
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
						if (!weakPointsArr.empty())
						{
							for (int i = 0; i < weakPointsArr.size(); i++)
							{
								weakPointsArr[i]->update(dt);
							}
						}

						if (dazeTimer.getElapsedTime().asSeconds() >= 5)
						{
							setRotate(player);
							this->setChargeCounter(0);
						}
					}
				}
			}

			if (velocityX < -0.5) velocityX = -0.5f;
			if (velocityX > 0.5) velocityX = 0.5f;

		}
		else if (phase == 2)
		{
			editWeakPoint(3.5f, 1.5f, player);

			if (!playerProjectiles.empty())
			{
				if (createTrigger)
				{
					weakPoints(playerProjectiles, "phase2", 4);
					createTrigger = false;
				}
			}
		}
		else if (phase == 3)
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
		if (phase == 1)
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

		this->collisionWithPlayer(player);
		


		//Trigger box
		if (!weakPointsArr.empty())
		{
			for (int i = 0; i < weakPointsArr.size(); i++)
			{
				weakPointsArr[i]->setPos(weakPoint);
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
		}
}

std::vector<Model*> EnemyBoss::getDebugModels()
{
	return debugWeakPointsBox;
}
