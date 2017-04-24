#include "Enemy.h"

void Enemy::initiate(int from)
{
	for (int i = from; i < this->CAP; i++)
	{
		this->enemyCharacters[i] = nullptr;
	}
}

void Enemy::expand()
{
	this->CAP += 10;
	EnemyChar* *temp = new EnemyChar*[this->CAP];
	for (int i = 0; i < this->nrOfEnemies; i++)
	{
		temp[i] = this->enemyCharacters[i];
	}
	delete[] this->enemyCharacters;
	this->enemyCharacters = temp;
}

void Enemy::freeMemory()
{
	for (int i = 0; i < this->nrOfEnemies; i++)
	{
		delete this->enemyCharacters[i];
	}
	delete[] this->enemyCharacters;
}

Enemy::Enemy()
{
	this->nrOfEnemies = 0;
	this->CAP = 10;
	this->enemyCharacters = new EnemyChar*[this->CAP];
	this->initiate();
	slimeModel = Model("models/Enemies/Slime/Slime.obj");
	toadModel = Model("models/Enemies/Toad/Toad.obj");
	batModel = Model("models/Enemies/Bat/BigBat.obj");
	batSmallModel = Model("models/Enemies/BatSmall/SmallBat.obj");
	bossModel = Model("models/cube/cube.obj");
	skeletonModel = Model("models/sphere/sphere.obj");
	crabModel = Model("models/Enemies/Crab/Crab.obj");
}

Enemy::~Enemy()
{
	this->freeMemory();
}

void Enemy::createSlime(glm::vec3 enemyStartPos)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemySlime(3, slimeModel, 1, enemyStartPos);
	this->nrOfEnemies++;
}

void Enemy::createToad(glm::vec3 enemyStartPos)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemyToad(5, toadModel, 2, enemyStartPos);
	this->nrOfEnemies++;
}

void Enemy::createGiantBat(glm::vec3 enemyStartPos)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemyBat(5, batModel, 2, enemyStartPos);
	this->nrOfEnemies++;
}

void Enemy::createBatSwarm(glm::vec3 enemyStartPos)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemyBatSmall(1, batSmallModel, 1, enemyStartPos);
	this->nrOfEnemies++;
}

void Enemy::createSkeleton(glm::vec3 enemyStartPos)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemyBat(10, batModel, 4, enemyStartPos);
	this->nrOfEnemies++;
}

void Enemy::createCrab(glm::vec3 enemyStartPos)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemyCrab(6, crabModel, 3, enemyStartPos);
	this->nrOfEnemies++;
}

void Enemy::createBoss(glm::vec3 enemyStartPos)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemyBoss(100, bossModel, 2, enemyStartPos);
	this->nrOfEnemies++;
}

void Enemy::sortEnemies(glm::vec3 playerPos)
{
	//Bubble sort
	glm::vec3 enemyPos1;
	glm::vec3 enemyPos2;
	bool sorted = false;
	while (!sorted)
	{
		sorted = true;
		for (int i = 0; i < this->nrOfEnemies - 1; i++)
		{
			enemyPos1 = this->enemyCharacters[i]->getEnemyPos();
			enemyPos2 = this->enemyCharacters[i + 1]->getEnemyPos();
			//Compare distance to enemy1 and distance to enemy2 and swap if out of order
			if (glm::distance(enemyPos1, playerPos) > glm::distance(enemyPos2, playerPos))
			{
				std::swap(enemyCharacters[i], enemyCharacters[i + 1]);
				sorted = false;
			}
		}
	}
}

glm::vec3 Enemy::getEnemyPos() const
{
	return enemyCharacters[0]->getEnemyPos();
}

int Enemy::getDamage() const
{
	return enemyCharacters[0]->getDamage();
}

void Enemy::update(float dt, glm::vec3 playerPos)
{
	sortEnemies(playerPos);

	for (int i = 0; i < nrOfEnemies; i++)
	{
		EnemyBatSmall *smallBatPtr = dynamic_cast<EnemyBatSmall*>(enemyCharacters[i]);
		if (smallBatPtr != nullptr)
		{
			smallBatsPos.push_back(enemyCharacters[i]);
		}
	}

	for (int i = 0; i < nrOfEnemies; i++)
	{
		enemyCharacters[i]->update(dt, playerPos, smallBatsPos);
	}
}

void Enemy::draw(Shader shader)
{
	for (int i = 0; i < nrOfEnemies; i++)
	{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &enemyCharacters[i]->getModelMatrix()[0][0]);
	enemyCharacters[i]->draw(shader);
	}
}
