#include "EnemyManager.h"


EnemyManager::EnemyManager()
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

EnemyManager::~EnemyManager()
{

}

void EnemyManager::createSlime(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemySlime(3, new Model(slimeModel), 1, enemyStartPos, glm::vec3(0.19f, 0.19f, 0.19f)));
}

void EnemyManager::createToad(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyToad(5, new Model(toadModel), 2, enemyStartPos, glm::vec3(0.18f, 0.18f, 0.18f)));
}

void EnemyManager::createGiantBat(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyBat(5, new Model(batModel), 2, enemyStartPos, glm::vec3(0.14f, 0.14f, 0.14f)));
}

void EnemyManager::createBatSwarm(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyBatSmall(1, new Model(batSmallModel), 1, enemyStartPos, glm::vec3(0.13f, 0.13f, 0.13f)));
	allSmallBats.push_back(allEnemies.back());
}

void EnemyManager::createSkeleton(glm::vec3 enemyStartPos, bool patrol)
{
	this->allEnemies.push_back(new EnemySkeleton(10, new Model(skeletonModel), 4, patrol, enemyStartPos, glm::vec3(0.12f, 0.12f, 0.12f)));
}

void EnemyManager::createCrab(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyCrab(6, new Model(crabModel), 3, enemyStartPos, glm::vec3(0.19f, 0.19f, 0.19f)));
}

void EnemyManager::createBoss(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyBoss(100, new Model(bossModel), 2, enemyStartPos, glm::vec3(0.10f, 0.10f, 0.10f)));
}

void EnemyManager::createFirefly(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyFireFly(1, new Model(fireflyModel), 2, enemyStartPos, glm::vec3(0.16f, 0.16f, 0.16f)));
}

void EnemyManager::clearDeadEnemies()
{
	for (int i = 0; i < this->allEnemies.size(); i++)
	{
		if (allEnemies[i]->getHealth() <= 0)
		{
			allEnemies.erase(allEnemies.begin() + i);
		}
	}
}

std::vector<Enemy*> &EnemyManager::getAllEnemies()
{
	return allEnemies;
}

void EnemyManager::update(float dt, int playerDamage, std::vector<Model*> &allModels, Player* player)
{
	clearDeadEnemies();
	std::vector<std::thread> allThreads;
	for (int i = 0; i < this->allEnemies.size(); i++)
	{
		if (allThreads.size() <= i)
		{
			allThreads.push_back(std::thread([&](Enemy * enemy) {enemy->update(dt, allSmallBats, allModels, player);}, allEnemies[i]));
		}
	}
	for (int i = 0; i < allThreads.size(); i++)
	{
		allThreads[i].join();
	}
}

void EnemyManager::draw(Shader shader)
{
	for (int i = 0; i < this->allEnemies.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &allEnemies[i]->getModel()->getModelMatrix()[0][0]);
		allEnemies[i]->draw(shader);
	}
}

void EnemyManager::removeAll()
{
	for (int i = 0; i < allEnemies.size(); i++)
	{
		delete allEnemies[i];
	}
	allEnemies.clear();

	allSmallBats.clear();
}
