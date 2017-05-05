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
	void sortEnemies(glm::vec3 playerPos);
	void clearDeadEnemies();
	glm::vec3 getPos()const;
	int getDamage()const;
	std::vector<Enemy*> &getAllEnemies();
	void update(float dt, glm::vec3 playerPos, int playerDamage, std::vector<Model*> &allModels, std::vector<glm::vec2> playerPoints);
	void draw(Shader shader);
	void removeAll();
};
#endif
