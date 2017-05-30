#include "EnemyManager.h"

EnemyManager::EnemyManager(SoundSystem * sound)
{
	slimeModel = new Model("models/Enemies/Slime/Slime.obj");
	toadModel = new Model("models/Enemies/Toad/Toad.obj");
	batModel = new Model("models/Enemies/Bat/BigBat.obj");
	batSmallModel = new Model("models/Enemies/BatSmall/SmallBat.obj");
	bossModel = new Model("models/Enemies/Boss/FullBoss.obj");
	skeletonModel = new Model("models/Enemies/Skeleton/Skeleton.obj");
	crabModel = new Model("models/Enemies/Crab/Crab.obj");
	fireflyModel = new Model("models/cube/cube.obj");
	allProjectiles = new std::vector<Projectile*>();

	this->sound = sound;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::createSlime(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemySlime(3, new Model(slimeModel), 1, 0.5, enemyStartPos, glm::vec3(2.8f, 2.8f, 2.8f), sound));
}

void EnemyManager::createToad(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyToad(5, new Model(toadModel), 2, 0.5, enemyStartPos, glm::vec3(3.30f, 3.30f, 3.30f), allProjectiles, sound));
}

void EnemyManager::createGiantBat(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyBat(5, new Model(batModel), 2, 0.5, enemyStartPos, glm::vec3(2.5f, 2.5f, 2.5f), sound));
}

void EnemyManager::createBatSwarm(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyBatSmall(1, new Model(batSmallModel), 1, 0.5, enemyStartPos, glm::vec3(2.8f, 2.8f, 2.8f), sound));
	allSmallBats.push_back(allEnemies.back());
}

void EnemyManager::createSkeleton(glm::vec3 enemyStartPos, bool patrol)
{
	this->allEnemies.push_back(new EnemySkeleton(10, new Model(skeletonModel), 4, 0.5, patrol, enemyStartPos, glm::vec3(1.8f, 1.8f, 1.8f), allProjectiles, sound));
}

void EnemyManager::createCrab(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyCrab(6, new Model(crabModel), 3, 0.5, enemyStartPos, glm::vec3(2.19f, 2.19f, 2.19f), sound));
}

void EnemyManager::createBoss(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyBoss(100, new Model(bossModel), 1, 1.8, enemyStartPos, glm::vec3(0.25f, 0.25f, 0.25f), allProjectiles, sound));
}

void EnemyManager::createFirefly(glm::vec3 enemyStartPos)
{
	this->allEnemies.push_back(new EnemyFireFly(1, new Model(fireflyModel), 2, 0.5, enemyStartPos, glm::vec3(5.16f, 5.16f, 5.16f), allProjectiles, sound));
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

bool EnemyManager::getBossKill() const
{
	EnemyBoss* enemyBossPtr = nullptr;
	for (int i = 0; i < this->allEnemies.size(); i++)
	{
		enemyBossPtr = dynamic_cast<EnemyBoss*>(allEnemies[i]);
		if (enemyBossPtr != nullptr)
		{
			return this->allEnemies[i]->getBossKill();
		}
	}
	return false;
}

glm::vec3 EnemyManager::getBossPos() const
{
	EnemyBoss* enemyBossPtr = nullptr;
	for (int i = 0; i < this->allEnemies.size(); i++)
	{
		enemyBossPtr = dynamic_cast<EnemyBoss*>(allEnemies[i]);
		if (enemyBossPtr != nullptr)
		{
			return this->allEnemies[i]->getPos();
		}
	}
	return glm::vec3(0.0f, 0.0f, 0.0f);
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
			allThreads.push_back(std::thread([&](Enemy * enemy) {enemy->update(dt, allSmallBats, allModels, player); }, allEnemies[i]));
		}
	}
	for (int i = 0; i < allThreads.size(); i++)
	{
		allThreads[i].join();
	}
	for (int i = 0; i < allProjectiles->size(); i++)
	{
		allProjectiles->at(i)->update(dt, allModels, player->getPos());
	}
	for (int i = 0; i < allProjectiles->size(); i++)
	{
		if (allProjectiles->at(i)->isInUse())
		{
			std::vector<glm::vec2> arrowPoints = allProjectiles->at(i)->getPoints();
			if (glm::distance(allProjectiles->at(i)->getPos(), player->getPos()) < 2.0f)
			{
				if (collision::collision(arrowPoints, player->getPoints()))
				{
					player->applyDamage(1);
					allProjectiles->at(i)->disableArrow();
				}
			}
		}
	}
}

void EnemyManager::draw(Shader shader)
{
	for (int i = 0; i < this->allEnemies.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &allEnemies[i]->getModel()->getModelMatrix()[0][0]);
		allEnemies[i]->draw(shader);
		EnemyBoss* boss = dynamic_cast<EnemyBoss*>(allEnemies[i]);
		if (boss != nullptr)
		{
			if (boss->getWeakPointActive() == true)
			{
				std::vector<Model*> temp = boss->getTriggerModels();
				for (int i = 0; i < temp.size(); i++)
				{
					glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &temp[i]->getModelMatrix()[0][0]);
					temp[i]->draw(shader);
				}
			}
		}
	}
	for (int i = 0; i < allProjectiles->size(); i++)
	{
		if (allProjectiles->at(i)->isInUse())
		{
			allProjectiles->at(i)->draw(shader);
		}
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
