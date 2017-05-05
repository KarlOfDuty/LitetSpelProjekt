#include "Enemy.h"


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
	this->allEnemies.push_back(new EnemySlime(3, new Model(slimeModel), 1, enemyStartPos));
}

void EnemyManager::createToad(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyToad(5, new Model(toadModel), 2, enemyStartPos));
}

void EnemyManager::createGiantBat(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyBat(5, new Model(batModel), 2, enemyStartPos));
}

void EnemyManager::createBatSwarm(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyBatSmall(1, new Model(batSmallModel), 1, enemyStartPos));
	allSmallBats.push_back(allEnemies.back());
}

void EnemyManager::createSkeleton(glm::vec3 enemyStartPos, bool patrol)
{
	this->allEnemies.push_back(new EnemySkeleton(10, new Model(skeletonModel), 4, patrol, enemyStartPos));
}

void EnemyManager::createCrab(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyCrab(6, new Model(crabModel), 3, enemyStartPos));
}

void EnemyManager::createBoss(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyBoss(100, new Model(bossModel), 2, enemyStartPos));
}

void EnemyManager::createFirefly(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyFireFly(1, new Model(fireflyModel), 2, enemyStartPos));
}

void EnemyManager::sortEnemies(glm::vec3 playerPos)
{
	//Bubble sort
	glm::vec3 enemyPos1;
	glm::vec3 enemyPos2;
	bool sorted = false;
	while (!sorted)
	{
		sorted = true;
		for (int i = 0; i < this->allEnemies.size()-1; i++)
		{
			enemyPos1 = this->allEnemies[i]->getPos();
			enemyPos2 = this->allEnemies[i + 1]->getPos();
			//Compare distance to enemy1 and distance to enemy2 and swap if out of order
			if (glm::distance(enemyPos1, playerPos) > glm::distance(enemyPos2, playerPos))
			{
				std::swap(allEnemies[i], allEnemies[i + 1]);
				sorted = false;
			}
		}
	}
}

void EnemyManager::clearDeadEnemies()
{
	for (int i = 0; i < this->allEnemies.size(); i++)
	{
		if (allEnemies[i]->getHealth() <= 0)
		{
			allEnemies.erase(allEnemies.begin() + i - 1);
		}
	}
}

glm::vec3 EnemyManager::getPos() const
{
	return allEnemies[0]->getPos();
}

int EnemyManager::getDamage() const
{
	return allEnemies[0]->getDamage();
}

std::vector<Enemy*> &EnemyManager::getAllEnemies()
{
	return allEnemies;
}

void EnemyManager::update(float dt, glm::vec3 playerPos, int playerDamage, std::vector<Model*> &allModels, std::vector<glm::vec2> playerPoints)
{
	//sortEnemies(playerPos);
	clearDeadEnemies();
	std::vector<std::thread> allThreads;
	for (int i = 0; i < this->allEnemies.size(); i++)
	{
		if (allThreads.size() <= i)
		{
			allThreads.push_back(std::thread([&](Enemy * enemy) {enemy->update(dt, playerPos, allSmallBats, allModels, playerPoints);}, allEnemies[i]));
		}
	}
	for (int i = 0; i < allThreads.size(); i++)
	{
		allThreads[i].join();
	}

	//EnemyManager taking damage
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
			if (fabs(getPos().x - playerPos.x) < 1.0 && fabs(getPos().y - playerPos.y) < 1.0)
			{
				allEnemies[0]->applyDamage(playerDamage);
			}
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
