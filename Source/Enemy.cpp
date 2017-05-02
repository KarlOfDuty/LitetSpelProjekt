#include "Enemy.h"


Enemy::Enemy()
{
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

}

void Enemy::createSlime(glm::vec3 enemyStartPos)
{
	this->enemyCharacters.push_back(new EnemySlime(3, new Model(slimeModel), 1, enemyStartPos));
}

void Enemy::createToad(glm::vec3 enemyStartPos)
{
	this->enemyCharacters.push_back(new EnemyToad(5, new Model(toadModel), 2, enemyStartPos));
}

void Enemy::createGiantBat(glm::vec3 enemyStartPos)
{
	this->enemyCharacters.push_back(new EnemyBat(5, new Model(batModel), 2, enemyStartPos));
}

void Enemy::createBatSwarm(glm::vec3 enemyStartPos)
{
	this->enemyCharacters.push_back(new EnemyBatSmall(1, new Model(batSmallModel), 1, enemyStartPos));
	smallBatsPos.push_back(enemyCharacters.back());
}

void Enemy::createSkeleton(glm::vec3 enemyStartPos, bool patrol)
{
	this->enemyCharacters.push_back(new EnemySkeleton(10, new Model(skeletonModel), 4, patrol, enemyStartPos));
}

void Enemy::createCrab(glm::vec3 enemyStartPos)
{
	this->enemyCharacters.push_back(new EnemyCrab(6, new Model(crabModel), 3, enemyStartPos));
}

void Enemy::createBoss(glm::vec3 enemyStartPos)
{
	this->enemyCharacters.push_back(new EnemyBoss(100, new Model(bossModel), 2, enemyStartPos));
}

void Enemy::createFirefly(glm::vec3 enemyStartPos)
{
	this->enemyCharacters.push_back(new EnemyFireFly(1, new Model(fireflyModel), 2, enemyStartPos));
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
		for (int i = 0; i < this->enemyCharacters.size()-1; i++)
		{
			enemyPos1 = this->enemyCharacters[i]->getPos();
			enemyPos2 = this->enemyCharacters[i + 1]->getPos();
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
	for (int i = 0; i < this->enemyCharacters.size(); i++)
	{
		if (enemyCharacters[i]->getHP() <= 0)
		{
			for (int j = (i + 1); j < this->enemyCharacters.size(); j++)
			{
				std::swap(enemyCharacters[j - 1], enemyCharacters[j]);
			}
			enemyCharacters.pop_back();
		}
	}
}

glm::vec3 Enemy::getPos() const
{
	return enemyCharacters[0]->getPos();
}

int Enemy::getDamage() const
{
	return enemyCharacters[0]->getDamage();
}

void Enemy::update(float dt, glm::vec3 playerPos, int playerDamage, std::vector<Model*> &allModels)
{
	sortEnemies(playerPos);
	enemyDead();
	for (int i = 0; i < this->enemyCharacters.size(); i++)
	{
		if (allThreads.size() <= i)
		{
			allThreads.push_back(std::thread([&](EnemyChar * enemy) {enemy->update(dt, playerPos, smallBatsPos, allModels);}, enemyCharacters[i]));
		}
		else
		{
			allThreads[i] = std::thread([&](EnemyChar * enemy) {enemy->update(dt, playerPos, smallBatsPos, allModels);}, enemyCharacters[i]);
		}
	}
	for (int i = 0; i < allThreads.size(); i++)
	{
		allThreads[i].join();
	}

	//Enemy taking damage
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
			if (fabs(getPos().x - playerPos.x) < 1.0 && fabs(getPos().y - playerPos.y) < 1.0)
			{
				enemyCharacters[0]->takingDamage(playerDamage);
			}
	}
}

void Enemy::draw(Shader shader)
{
	for (int i = 0; i < this->enemyCharacters.size(); i++)
	{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &enemyCharacters[i]->getModelMatrix()[0][0]);
	enemyCharacters[i]->draw(shader);
	}
}
