#include "EnemyBat.h"
#include "Player.h"
#include "Trigger.h"

EnemyBat::EnemyBat(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, SoundSystem * sound) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor, sound)
{
	swoopAttack = true;
	startPosition = enemyStartPos;
	returnToStart = false;
	newCheckPoint = enemyStartPos;
	this->sound = sound;
}

EnemyBat::~EnemyBat()
{

}

void EnemyBat::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBat::updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
{
	groundCheck();

	std::vector<Model*> sortedModels;
	for (int i = 0; i < allModels.size(); i++)
	{
		glm::vec3 min, max;
		allModels[i]->getScaledMinMaxBouding(min, max);
		min += allModels[i]->getPos();
		max += allModels[i]->getPos();
		if (getPos().x >= min.x && getPos().x <= max.x)
		{
			sortedModels.push_back(allModels[i]);
		}
	}
	//Now find the ground based on the sorted models
	bool foundGround = false;
	float closestDistance = 100000;
	glm::vec3 rayOrigin = glm::vec3(getPos().x, getPos().y, getPos().z);
	glm::vec3 rayDir(0, -1, 0);
	for (int i = 0; i < sortedModels.size(); i++)
	{
		glm::vec3 aabbMin, aabbMax;
		sortedModels[i]->getMinMaxBouding(aabbMin, aabbMax);
		glm::vec3 scale;
		glm::decompose(sortedModels[i]->getModelMatrix(), scale, glm::quat(), glm::vec3(), glm::vec3(), glm::vec4());
		aabbMin = aabbMin * scale * scale;
		aabbMax = aabbMax * scale * scale;
		glm::mat4 boxMat = sortedModels[i]->getModelMatrix();
		float distance = 10000;

		//Raycast downwards to find the distance to ground
		if (collision::TestRayOBBIntersection(rayOrigin, rayDir, aabbMin, aabbMax, boxMat, distance))
		{
			if (distance < closestDistance)
			{
				closestDistance = distance;
				foundGround = true;
			}
		}
	}
	if (foundGround)
	{
		groundPos = rayOrigin.y - closestDistance;
	}
	else
	{
		groundPos = 0;
	}

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
		if (collidedFrom.y == 0 && collidedFrom.x != 0)
		{
			velocityY += 90.0f*dt;
		}
		if (collidedFrom.y < 0 && collidedFrom.x != 0)
		{
			newCheckPoint.y = enemyPosCurrent.y;
			newCheckPoint.x = enemyPosCurrent.x;
		}
		if (collidedFrom.y > 0)
		{
			goUp = true;
		}

		if (collidedFrom.y < 0)
		{
			newCheckPoint.y = enemyPosCurrent.y;
			newCheckPoint.x = enemyPosCurrent.x;
		}

		if (collisionTime.getElapsedTime().asSeconds() >= 6)
		{
			returnToStart = true;
			playerSeen = false;
		}
	}
	else
	{
		collisionTime.restart();
	}

	if (enemyPosCurrent.y <= groundPos+60)
	{
		goUp = true;
	}

	if (fabs(enemyPosCurrent.x - newCheckPoint.x) < 10.5f)
	{
		xPointReached = true;
	}

	if (fabs(enemyPosCurrent.y - newCheckPoint.y) < 10.5f)
	{
		yPointReached = true;
	}

	if (yPointReached && !xPointReached)
	{
		xPointReached = true;
	}

	if (xPointReached && yPointReached)
	{
		checkPointReached = true;
		goingLeft = false;
		goingRight = false;
		goUp = false;
		swoopAttack = false;
	}
	
	if (!xPointReached && !yPointReached)
	{
		checkPointReached = false;
		waitInAir.restart();
	}

	if (playerSeen)
	{
		if (waitInAir.getElapsedTime().asSeconds() >= 1.2)
		{
			if (checkPointReached)
			{
				if (enemyPosCurrent.x > player->getPos().x)
				{
					goingLeft = true;
				}
				else if (enemyPosCurrent.x < player->getPos().x)
				{
					goingRight = true;
				}

				if (goingLeft)
				{
					newCheckPoint.x = newCheckPoint.x - 200.0f;
					newCheckPoint.y = startPosition.y;
					newCheckPoint.z = startPosition.z;
				}

				if (goingRight)
				{
					newCheckPoint.x = newCheckPoint.x + 200.0f;
					newCheckPoint.y = startPosition.y;
					newCheckPoint.z = startPosition.z;
				}
				swoopAttack = true;
				xPointReached = false;
				yPointReached = false;
			}
		}
	}

	

	//Detect player
	if (glm::length(enemyPosCurrent - player->getPos()) < 250.0f)
	{
		playerSeen = true;
		returnToStart = false;
	}

	if (playerSeen == true && soundTimer.getElapsedTime().asSeconds() > 6)
	{
		this->sound->playSound("screees");
		soundTimer.restart();
	}

	//Move
	//movement calculations
	midX = startPosition.y - 400;
	float curve = glm::pow(enemyPosCurrent.y - midX, 0.8);
	//
	if (!returnToStart)
	{
		if (swoopAttack)
		{
			if (playerSeen)
			{
				if (fabs(enemyPosCurrent.x - newCheckPoint.x) > 1.1f && goingLeft && !goUp)
				{
					velocityX -= 100.0f*dt;
					velocityY -= curve*dt;
				}
				else if (fabs(enemyPosCurrent.x - newCheckPoint.x) > 1.1f && goingRight && !goUp)
				{
					velocityX += 100.0f*dt;
					velocityY -= curve*dt;
				}
				if (fabs(enemyPosCurrent.y - newCheckPoint.y) > 1.1f && goingLeft && goUp)
				{
					velocityX -= 100.0f*dt;
					velocityY += curve*dt;
				}
				else if (fabs(enemyPosCurrent.y - newCheckPoint.y) > 1.1f && goingRight && goUp)
				{
					velocityX += 100.0f*dt;
					velocityY += curve*dt;
				}
			}
		}
	}
	else
	{
		if (glm::length(enemyPosCurrent - startPosition) > 15.5f)
		{
			if (!collides)
			{
				if (enemyPosCurrent.x > startPosition.x)
				{
					velocityX -= 70.5f*dt;
				}
				else if (enemyPosCurrent.x < startPosition.x)
				{
					velocityX += 70.5f*dt;
				}
				if (enemyPosCurrent.y > startPosition.y)
				{
					velocityY -= 70.5f*dt;
				}
				else if (enemyPosCurrent.y < startPosition.y)
				{
					velocityY += 70.5f*dt;
				}
			}
			else
			{
				velocityY += 80.0f*dt;
			}
		}
		else
		{
			returnToStart = false;
			playerSeen = false;
		}
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

