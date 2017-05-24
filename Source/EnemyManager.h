#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H
#include "EnemySlime.h"
#include "EnemyToad.h"
#include "EnemyBat.h"
#include "EnemyBatSmall.h"
#include "EnemyBoss.h"
#include "EnemySkeleton.h"
#include "EnemyCrab.h"
#include "EnemyFireFly.h"
#include "Player.h"
#include "Shader.h"
#include "Projectile.h"
#include <SFML\Window.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <iostream>
#include <thread>

class EnemyManager
{
private:
	std::vector<Enemy*> allEnemies;
	std::vector<Enemy*> allSmallBats;
	std::vector<Projectile*> *allProjectiles;
	Model* slimeModel;
	Model* toadModel;
	Model* batModel;
	Model* bossModel;
	Model* batSmallModel;
	Model* skeletonModel;
	Model* crabModel;
	Model* fireflyModel;
public:
	EnemyManager();
	~EnemyManager();
	void createSlime(glm::vec3 enemyStartPos);
	void createToad(glm::vec3 enemyStartPos);
	void createGiantBat(glm::vec3 enemyStartPos);
	void createBatSwarm(glm::vec3 enemyStartPos);
	void createSkeleton(glm::vec3 enemyStartPos, bool patrol);
	void createCrab(glm::vec3 enemyStartPos);
	void createBoss(glm::vec3 enemyStartPos);
	void createFirefly(glm::vec3 enemyStartPos);
	void clearDeadEnemies();
	bool getBossKill()const;
	glm::vec3 getBossPos()const;
	std::vector<Enemy*> &getAllEnemies();
	void update(float dt, int playerDamage, std::vector<Model*> &allModels, Player* player);
	void draw(Shader shader);
	void removeAll();
};
#endif
