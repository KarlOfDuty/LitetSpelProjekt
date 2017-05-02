#include "EnemyBat.h"

EnemyBat::EnemyBat(int health, Model* model, int damage, glm::vec3 enemyStartPos) :Enemy(health, model, damage, enemyStartPos)
{
	findPlayer = true;
}

EnemyBat::~EnemyBat()
{

}

void EnemyBat::attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent)
{

}

void EnemyBat::updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels)
{
	groundCheck();

	if (goingRight)
	{
		checkpoint.x = playerPos.x + 12.0f;
		checkpoint.y = playerPos.y + 7.0f;
		checkpoint.z = playerPos.z;
	}
	if (goingLeft)
	{
		checkpoint.x = playerPos.x - 12.0f;
		checkpoint.y = playerPos.y + 7.0f;
		checkpoint.z = playerPos.z;
	}

	if (findPlayer)
	{
		if (fabs(enemyPosCurrent.x - playerPos.x) < 2.0f && fabs(enemyPosCurrent.y - playerPos.y) < 2.0f)
		{
			findPlayer = false;
			clockRestart = true;
		}
	}

	if (!findPlayer)
	{
		if (fabs(enemyPosCurrent.x - checkpoint.x) < 5.0f && fabs(enemyPosCurrent.y - checkpoint.y) < 5.0f)
		{
			if (clockRestart)
			{
				waitInAir.restart();
				clockRestart = false;
			}
			if (waitInAir.getElapsedTime().asSeconds() >= 0.9)
			{
				findPlayer = true;
			}
		}
	}

	//Move
	midX = playerPos.y;
	float curve = glm::pow(enemyPosCurrent.y - midX, 0.3);
	if (findPlayer == true)
	{
		if (glm::length(enemyPosCurrent - playerPos) < 10.0f || playerSeen)
		{
			if (enemyPosCurrent.x > playerPos.x)
			{
				velocityX -= 2.6f*dt;
				velocityY -= curve*dt;
				goingLeft = true;
				goingRight = false;
			}
			else if (enemyPosCurrent.x < playerPos.x)
			{
				velocityX += 2.6f*dt;
				velocityY -= curve*dt;
				goingRight = true;
				goingLeft = false;
			}
			playerSeen = true;
		}
	}
	else
	{
		if (enemyPosCurrent.x > checkpoint.x)
		{
			velocityX -= 2.6f*dt;
		}
		else if (enemyPosCurrent.x < checkpoint.x)
		{
			velocityX += 2.6f*dt;
			
		}
		if (enemyPosCurrent.y > checkpoint.y)
		{
			velocityY -= curve*dt;
		}
		else if (enemyPosCurrent.y < checkpoint.y)
		{
			velocityY += curve*dt;
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
	/*if (enemyPosCurrent.y <= 0) {
		enemyPosCurrent.y = 0;
		isOnGround = true;
	}*/

	setPos(enemyPosCurrent);
	collision(allModels);
}

