#include "EnemyBoss.h"
#include "Player.h"
#include "Trigger.h"

EnemyBoss::EnemyBoss(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor)
{
	this->allProjectiles = allProjectiles;
	this->acceleration = 0.4f;
	this->originPoint = enemyStartPos;
	this->attacking = true;
	this->phase = 1;
	this->chargeCounter = 0;
	this->createTrigger = true;
	this->rotateNow = false;
	bossImmunity = true;
	this->centerOfRoom = enemyStartPos;
	this->wallDestroyed = false;
	
	projectileModel = new Model("models/sphere/sphere.obj");
	boxModel = new Model("models/cube/cube.obj");

	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

EnemyBoss::~EnemyBoss()
{

}

void EnemyBoss::setWaterArea(Player * player, std::vector<Model*> &allModels)
{
	allModels.erase(allModels.begin() + 0);

	playerAndBoss.push_back(this);
	playerAndBoss.push_back(player);

	std::vector<glm::vec2> corners1 = { glm::vec2(31.5f, 1.0f), glm::vec2(31.5f, 10.0f), glm::vec2(60.0f,1.0f), glm::vec2(60.0f, 10.0f) };
	TriggerSettings settings3;
	settings3.onEnter = true;
	settings3.onExit = true;
	waterArea.push_back(new Trigger(corners1, settings3, player, playerAndBoss, "playerUnderBoss"));
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

void EnemyBoss::setRotateNow()
{
	rotateNow = true;
}

void EnemyBoss::loseTrackOfPlayer(bool playerIsFound)
{
	playerTracked = playerIsFound;
}

void EnemyBoss::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{
	if (attackCooldown.getElapsedTime().asSeconds() >= 1)
	{
		float rotation = -atan2(getPos().x - playerPos.x, getPos().y - playerPos.y-2);
		glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));

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
						allProjectiles->at(i)->shoot(projectileModel, getPos(), direction, glm::vec2(0, 0), 15.f, glm::vec3(0.2, 0.2, 0.2), false, true);
						i = (int)allProjectiles->size();
					}
				}
			}
		}
		else
		{
			Projectile* temp = new Projectile;
			temp->shoot(projectileModel, getPos(), direction, glm::vec2(0, 0), 15.f, glm::vec3(0.2, 0.2, 0.2), false, true);
			allProjectiles->push_back(temp);
		}
		attackCooldown.restart();
	}
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
							setRotateNow();
							this->setChargeCounter(0);
						}
						if (rotateNow)
						{
							setRotate(player);
							rotateNow = false;
						}
						
					}
				}
			}

			if (velocityX < -0.5) velocityX = -0.5f;
			if (velocityX > 0.5) velocityX = 0.5f;

		}
		else if (phase == 2)
		{
			editWeakPoint(2.0f, -1.0f, player);

			if (!playerProjectiles.empty())
			{
				if (createTrigger)
				{
					weakPoints(playerProjectiles, "phase2", 4);
					createTrigger = false;
				}
			}

			if (!wallDestroyed)
			{
				if (collidedFrom.x == 0)
				{
					velocityX = velocityX + acceleration * dt;
				}
				if (collidedFrom.x < 0)
				{
					velocityX = 0;
					wallDestroyed = true;
					setWaterArea(player, allModels);
					removeGroundTimer.restart();
					removeGround = true;
				}
			}

			if (wallDestroyed)
			{
				if (removeGroundTimer.getElapsedTime().asSeconds() >= 1.5 && removeGround)
				{
					allModels.erase(allModels.begin() + 0, allModels.begin() + 5);
					allModels[0]->setModelMatrix({
						15.0, 0.0, 0.0, 0.0,
						0.0, 5.0, 0.0, 0.0,
						0.0, 0.0, 5.0, 0.0,
						45.0, 8.0, 0.0, 1.0
					});
					removeGround = false;
				}

				//If boss is outside center go to center
				if (glm::length(enemyPosCurrent.x - centerOfRoom.x) > 0.5f)
				{
					if (getPos().x >= centerOfRoom.x)
					{
						rotateLeft = false;
					}
					else if (getPos().x <= centerOfRoom.x)
					{
						rotateLeft = true;
					}
					if (enemyPosCurrent.x > centerOfRoom.x)
					{
						velocityX -= 3.5f*dt;
					}
					else if (enemyPosCurrent.x < centerOfRoom.x)
					{
						velocityX += 3.5f*dt;
					}
				}

				//If boss is in center
				if (glm::length(enemyPosCurrent.x - centerOfRoom.x) < 0.5f)
				{
					if (!weakPointsArr.empty())
					{
						for (int i = 0; i < weakPointsArr.size(); i++)
						{
							weakPointsArr[i]->update(dt);
						}
					}

					if (playerTracked)
					{
						setRotate(player);
						this->attackPlayer(dt, player->getPos(), enemyPosCurrent);
						oldPlayerPos = player->getPos();
					}
					else if (!playerTracked)
					{
						this->attackPlayer(dt, oldPlayerPos, enemyPosCurrent);
					}
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
		else if (phase == 2)
		{
			if (!wallDestroyed)
			{
				enemyPosCurrent.x += velocityX;
				enemyPosCurrent.y += velocityY*dt;
			}
			if (wallDestroyed)
			{
				enemyPosCurrent.x += velocityX;
				velocityX = 0;
				enemyPosCurrent.y += velocityY*dt;
			}
		}
		else if (phase == 3)
		{

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
		if (!waterArea.empty())
		{
			for (int i = 0; i < weakPointsArr.size(); i++)
			{
				waterArea[i]->update(dt);
			}
		}

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
