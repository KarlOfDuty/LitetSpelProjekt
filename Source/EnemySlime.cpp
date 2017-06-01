#include "EnemySlime.h"
#include "Player.h"
#include "Trigger.h"

EnemySlime::EnemySlime(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, SoundSystem * sound) :Enemy(health, model, damage, immunityTime, enemyStartPos, scaleFactor, sound)
{
	startPosition = enemyStartPos;
	returnToStart = false;
	this->sound = sound;
}

EnemySlime::~EnemySlime()
{

}

void EnemySlime::attackPlayer(float dt, glm::vec3 playerPos)
{

}

void EnemySlime::update(float dt, std::vector<Enemy*> &allSmallBats, std::vector<Model*> &allModels, Player* player)
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

	//Patrol check 
	if (pos.x < checkPoint.x-40)
	{
		checkPointReached = true;
		
	}
	else if (pos.x > checkPoint.x+40)
	{
		checkPointReached = false;
	}

	if (collides)
	{
		if (collidedFrom.x != 0 && collidedFrom.y > 0)
		{
			velocityY = 150;
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
	
	if(collidedFrom.x == 0)
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
		this->sound->playSound("goopySlimeSounds");
		soundTimer.restart();
	}

	//Move
	if (!returnToStart)
	{
		if (playerSeen)
		{
			if (pos.x >= player->getPos().x)
			{
				rotateLeft = false;
			}
			else if (pos.x < player->getPos().x)
			{
				rotateLeft = true;
			}

			if (pos.x > player->getPos().x)
			{
				velocityX -= 40.0f*dt;
			}
			else
			{
				velocityX += 40.0f*dt;
			}
		}
		else
		{
			//Patrol
			if (pos.x >= checkPoint.x)
			{
				rotateLeft = false;
			}
			else if (pos.x < checkPoint.x)
			{
				rotateLeft = true;
			}

			if (checkPointReached == false)
			{
				velocityX -= 40.0f*dt;
			}
			else if (checkPointReached == true)
			{
				velocityX += 40.0f*dt;
			}
		}
	}
	else
	{
		if (pos.x >= startPosition.x)
		{
			rotateLeft = false;
		}
		else if (pos.x < startPosition.x)
		{
			rotateLeft = true;
		}

		if (glm::length(pos.x - startPosition.x) > 20.0f)
		{
			if (pos.x > startPosition.x)
			{
				velocityX -= 40.0f*dt;
			}
			else if (pos.x < startPosition.x)
			{
				velocityX += 40.0f*dt;
			}
		}
		else
		{
			returnToStart = false;
			playerSeen = false;
		}
	}

	if (collidingWithGround && collisionWithPlayer(player))
	{
		velocityY = 100;
	}
	else if (!collidingWithGround)
	{
		velocityY -= 300*dt;
	}
	//Maximum falling speed
	if (velocityY < -300)
	{
		velocityY = -300;
	}

	//Apply velocity
	pos.x += velocityX;
	velocityX = 0;
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

	if (!rotateLeft)
	{
		rotateModel(-90.0f);
	}
	else
	{
		rotateModel(90.0f);
	}
}

