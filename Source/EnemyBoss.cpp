#include "EnemyBoss.h"
#include "Player.h"
#include "Trigger.h"

EnemyBoss::EnemyBoss(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles, SoundSystem * sound) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor, sound)
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
	this->wallDestroyed = false;

	this->sound = sound;
	this->sound1 = true;
	this->sound2 = true;
	this->sound3 = true;

	this->inRightCorner = true;
	this->blockExit = false;
	this->isChandelierCreated = false;
	this->platformCreated = false;
	this->moveChandelier = false;
<<<<<<< HEAD
	this->weakPointActive = false;
	this->playerInWater = false;
=======

>>>>>>> refs/remotes/origin/master
	
	projectileModel = new Model("models/sphere/sphereFire.obj");
	boxModel = new Model("models/cube/cube.obj");
	
	glm::mat4 hideModelMatrix({
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,10000,1
	});

	chandelierModel = new Model("models/cube/cube.obj", hideModelMatrix);
	weakPointModel = new Model("models/sphere/sphereRed.obj", hideModelMatrix);

	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	corners.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

EnemyBoss::~EnemyBoss()
{

}

void EnemyBoss::setChandelierButton(Player* player)
{
	std::vector<glm::vec2> corners = { glm::vec2(20.0f, 26.0f), glm::vec2(20.0f, 28.0f), glm::vec2(22.0f,26.0f), glm::vec2(22.0f, 28.0f) };
	TriggerSettings buttonSettings;
	buttonSettings.onEnter = true;
	buttonSettings.numberOfActivationsAllowed = 1;
	chandelierButton.push_back(new Trigger(corners, buttonSettings, player, this, "finishingBlow"));
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

void EnemyBoss::setPlayerInWater(bool isInWater)
{
	this->playerInWater = isInWater;
}

void EnemyBoss::setCreateTrigger(bool createTrigger)
{
	this->createTrigger = createTrigger;
}

void EnemyBoss::setChargeCounter(int amountOfCharges)
{
	this->chargeCounter = amountOfCharges;
}

void EnemyBoss::setAttacking(bool isAttacking)
{
	this->attacking = isAttacking;
	walkTimer.restart();
}

void EnemyBoss::setRotateToPlayer(Player* player)
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

void EnemyBoss::setRotateToOrigin()
{
	if (getPos().x >= originPoint.x)
	{
		rotateLeft = false;
	}
	if (getPos().x <= originPoint.x)
	{
		rotateLeft = true;
	}
}

void EnemyBoss::setRotateNow()
{
	rotateNow = true;
}

void EnemyBoss::setChandelierMove()
{
	moveChandelier = true;
	timeBeforeDeath.restart();
	setBossImmunity(false);
}

void EnemyBoss::loseTrackOfPlayer(bool playerIsFound)
{
	playerTracked = playerIsFound;
}

bool EnemyBoss::getWeakPointActive()
{
	return this->weakPointActive;
}

int EnemyBoss::getPhase() const
{
	return this->phase;
}

bool EnemyBoss::getPlayerTracked() const
{
	return this->playerTracked;
}

bool EnemyBoss::getPlayerInWater() const
{
	return this->playerInWater;
}

void EnemyBoss::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{
	if (attackCooldown.getElapsedTime().asSeconds() >= 0.5)
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
						allProjectiles->at(i)->shoot(projectileModel, getPos(), direction, glm::vec2(0, 0), 15.f, glm::vec3(0.4, 0.4, 0.4), false, true);
						i = (int)allProjectiles->size();
					}
				}
			}
		}
		else
		{
			Projectile* temp = new Projectile;
			temp->shoot(projectileModel, getPos(), direction, glm::vec2(0, 0), 15.f, glm::vec3(0.4, 0.4, 0.4), false, true);
			allProjectiles->push_back(temp);
		}
		attackCooldown.restart();
	}
}

void EnemyBoss::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
{

	groundCheck();
	if (sound1 == true && phase == 1)
	{
		this->sound->playSound("wierdScreemFromBoss");
		sound1 = false;
	}
	if (sound2 == true && phase == 2)
	{
		this->sound->playSound("wierdScreemFromBoss");
		sound2 = false;
	}
	if (sound3 == true && phase == 3)
	{
		this->sound->playSound("wierdScreemFromBoss");
		sound3 = false;
	}
	if (collidedFrom.y > 0)
	{
		collidingWithGround = true;
	}
	else if (collidedFrom.y <= 0)
	{
		collidingWithGround = false;
	}

	if (!isChandelierCreated)
	{
		setChandelierButton(player);
		chandelierPos = allModels.size();
		allModels.push_back(new Model(boxModel,
		{
			4.0, 0.0, 0.0, 0.0,
			0.0, 2.0, 0.0, 0.0,
			0.0, 0.0, 4.0, 0.0,
			45.0, 28.0, 0.0, 1.0
		}));
		isChandelierCreated = true;
	}

	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 10.0f)
	{
		if (!blockExit)
		{
			allModels.push_back(new Model(boxModel,
			{
				4.0, 0.0, 0.0, 0.0,
				0.0, 5.0, 0.0, 0.0,
				0.0, 0.0, 5.0, 0.0,
				30.5, 16.0, 0.0, 1.0
			}));
			blockExit = true;
		}
		playerSeen = true;
	}

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
			if (!playerProjectiles.empty())
			{
				if (createTrigger)
				{
					weakPoints(playerProjectiles, "phase1", 3);
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
					attacking = false;
					movingRight = false;
					movingLeft = false;
					velocityX = 0;
					walkTimer.restart();
					chargeCounter = chargeCounter + 1;
				}
			}

			if (playerSeen)
			{
				if (collidingWithGround)
				{
					//Charge
					if (chargeCounter < 3)
					{
						this->weakPointActive = false;
						if (!attacking)
						{
							setRotateToOrigin();
						}

						if (attacking)
						{
							if (!movingRight)
							{
								if (enemyPosCurrent.x >= originPoint.x)
								{
									movingLeft = true;
								}
							}
							if (!movingLeft)
							{
								if (enemyPosCurrent.x < originPoint.x)
								{
									movingRight = true;
								}
							}
							if (movingLeft)
							{
								velocityX = velocityX - acceleration * dt;
							}
							else if (movingRight)
							{
								velocityX = velocityX + acceleration * dt;
							}
						}
						dazeTimer.restart();
					}

					if (chargeCounter >= 3)
					{
						this->weakPointActive = true;
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
							setRotateToOrigin();
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
			if (!attacking)
			{
				if (walkTimer.getElapsedTime().asSeconds() >= 1.0)
				{
					attacking = true;
				}
			}

			if (!playerProjectiles.empty())
			{
				if (createTrigger)
				{
					weakPoints(playerProjectiles, "phase2", 3);
					createTrigger = false;
				}
			}

			if (attacking)
			{
				if (collidedFrom.x != 0)
				{
					attacking = false;
					movingRight = false;
					movingLeft = false;
					velocityX = 0;
					walkTimer.restart();
					chargeCounter = chargeCounter + 1;
				}
			}

			if (!wallDestroyed)
			{
				this->weakPointActive = false;
				if (!attacking)
				{
					setRotateToOrigin();
				}

				if (attacking)
				{
					if (inRightCorner)
					{
						if (!movingRight)
						{
							if (enemyPosCurrent.x >= originPoint.x)
							{
								movingLeft = true;
								inRightCorner = true;
							}
						}
					}

					if (!movingLeft)
					{
						if (enemyPosCurrent.x < originPoint.x)
						{
							movingRight = true;
							inRightCorner = false;
						}
					}
					if (movingLeft)
					{
						velocityX = velocityX - acceleration * dt;
					}
					else if (movingRight)
					{
						velocityX = velocityX + acceleration * dt;
					}
				}

				if (collidedFrom.x < 0 && !inRightCorner)
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
				if (glm::length(enemyPosCurrent.x - originPoint.x) > 0.5f)
				{
					if (getPos().x > originPoint.x)
					{
						rotateLeft = false;
					}
					else if (getPos().x <= originPoint.x)
					{
						rotateLeft = true;
					}
					if (enemyPosCurrent.x >= originPoint.x)
					{
						velocityX -= 3.5f*dt;
					}
					else if (enemyPosCurrent.x < originPoint.x)
					{
						velocityX += 3.5f*dt;
					}
				}

				//If boss is in center
				if (glm::length(enemyPosCurrent.x - originPoint.x) < 0.5f)
				{
					if (!weakPointsArr.empty())
					{
						this->weakPointActive = true;
						for (int i = 0; i < weakPointsArr.size(); i++)
						{
							weakPointsArr[i]->update(dt);
						}
					}

					if (!playerInWater)
					{
						if (!rotateLeft)
						{
							if (enemyPosCurrent.x >= player->getPos().x)
							{
								playerTracked = true;
							}
						}
						if (
							rotateLeft)
						{
							if (enemyPosCurrent.x < player->getPos().x)
							{
								playerTracked = true;
							}
						}
					}

					if (playerTracked)
					{
						setRotateToPlayer(player);
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
			editWeakPoint(-100.0f, -101.0f, player);

			if (!playerProjectiles.empty())
			{
				if (createTrigger)
				{
					weakPoints(playerProjectiles, "phase1", 3);
					createTrigger = false;
				}
			}

			if (!platformCreated)
			{
				chandelierPos -= 6;
				allModels.push_back(new Model(boxModel,
				{
					3.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 5.0, 0.0,
					33.0, 16.0, 0.0, 1.0
				}));
				platformCreated = true;
			}

			this->weakPointActive = true;

			if (moveChandelier)
			{
				if (timeBeforeDeath.getElapsedTime().asSeconds() < 3.0)
				{
					glm::mat4 modelMat = allModels[chandelierPos]->getModelMatrix();
					modelMat[3].y -= 5 * dt;
					allModels[chandelierPos]->setModelMatrix(modelMat);
					this->bossKill = true;
					applyDamage(30);
					setBossImmunity(true);
					if (timeBeforeDeath.getElapsedTime().asSeconds() >= 2.0)
					{
						if (scaleFactor.y > 0.05)
						{
							scaleFactor.y -= 0.2*dt;
							scaleFactor.x += 0.2*dt;
							enemyPosCurrent.x -= 1 * dt;
						}
					}
				}
			}

			if (this->getHealth() > 10)
			{
				if (playerTracked)
				{
					setRotateToPlayer(player);
					this->attackPlayer(dt, player->getPos(), enemyPosCurrent);
					oldPlayerPos = player->getPos();
				}
				else if (!playerTracked)
				{
					this->attackPlayer(dt, oldPlayerPos, enemyPosCurrent);
				}
			}
			else if (this->getHealth() <= 10)
			{
				if (!weakPointsArr.empty())
				{
					this->weakPointsArr.clear();
				}
			}
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

		if (phase == 1)
		{
			editWeakPoint(3.5f, -2.0f, player);
		}
		else if (phase == 2)
		{
			editWeakPoint(2.0f, -1.2f, player);
		}

		if (this->getHealth() > 10)
		{
			this->collisionWithPlayer(player);
		}

		//Trigger box
		if (!chandelierButton.empty())
		{
			for (int i = 0; i < chandelierButton.size(); i++)
			{
				chandelierButton[i]->update(dt);
			}
		}

		if (!waterArea.empty())
		{
			for (int i = 0; i < waterArea.size(); i++)
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
		}

		glm::vec2 center(0,0);
		for (int i = 0; i < weakPoint.size(); i++)
		{
			center += weakPoint[i];
		}
		center = center / (float)weakPoint.size();
		glm::mat4 ModelMat({
			0.3,0,0,0,
			0,0.3,0,0,
			0,0,0.3,0,
			center.x,center.y,0,1
		});
		weakPointModel->setModelMatrix(ModelMat);

		center = glm::vec2();
		std::vector<glm::vec2> chandelierPoints = chandelierButton[0]->getPoints();
		for (int i = 0; i < chandelierPoints.size(); i++)
		{
			center += chandelierPoints[i];
		}
		center = center / (float)chandelierPoints.size();
		ModelMat = glm::mat4({
			2,0,0,0,
			0,2,0,0,
			0,0,2,0,
			center.x,center.y,0,1
		});
		chandelierModel->setModelMatrix(ModelMat);
}

std::vector<Model*> EnemyBoss::getTriggerModels()
{
	std::vector<Model*> triggerModels;
	triggerModels.push_back(weakPointModel);
	if (phase == 3)
	{
		triggerModels.push_back(chandelierModel);
	}
	return triggerModels;
}
