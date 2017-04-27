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
	slimeModel = new Model("models/Enemies/Slime/Slime.obj");
	toadModel = new Model("models/Enemies/Toad/Toad.obj");
	batModel = new Model("models/Enemies/Bat/BigBat.obj");
	batSmallModel = new Model("models/Enemies/BatSmall/SmallBat.obj");
	bossModel = new Model("models/cube/cube.obj");
	skeletonModel = new Model("models/Enemies/Skeleton/Skeleton.obj");
	crabModel = new Model("models/Enemies/Crab/Crab.obj");
	fireflyModel = new Model("models/cube/cube.obj");
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

void Enemy::createSkeleton(glm::vec3 enemyStartPos, bool patrol)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemySkeleton(10, skeletonModel, 4, patrol, enemyStartPos);
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

void Enemy::createFirefly(glm::vec3 enemyStartPos)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemyFireFly(1, fireflyModel, 2, enemyStartPos);
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

void Enemy::enemyDead()
{
	for (int i = 0; i < this->nrOfEnemies; i++)
	{
		if (enemyCharacters[i]->getHP() <= 0)
		{
			for (int j = (i + 1); j < this->nrOfEnemies; j++)
			{
				std::swap(enemyCharacters[j - 1], enemyCharacters[j]);
			}
			nrOfEnemies--;
			delete enemyCharacters[nrOfEnemies];
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

void Enemy::update(float dt, glm::vec3 playerPos, int playerDamage, std::vector<Model*> &allModels)
{
	sortEnemies(playerPos);
	enemyDead();

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
			enemyCharacters[i]->update(dt, playerPos, smallBatsPos, allModels);
	}

	//Enemy taking damage
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
			if (fabs(getEnemyPos().x - playerPos.x) < 1.0 && fabs(getEnemyPos().y - playerPos.y) < 1.0)
			{
				enemyCharacters[0]->takingDamage(playerDamage);
			}
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
