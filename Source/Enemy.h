#ifndef ENEMY_H
#define ENEMY_H
#include "EnemySlime.h"
#include "EnemyToad.h"
#include "EnemyBat.h"
#include "EnemyBatSmall.h"
#include "EnemyBoss.h"
#include "EnemySkeleton.h"
#include "Player.h"
#include "Shader.h"
#include <SFML\Window.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <iostream>

class Enemy
{
private:
	int nrOfEnemies;
	int CAP;
	EnemyChar* *enemyCharacters;
	void initiate(int from = 0);
	void expand();
	void freeMemory();
	glm::vec3 enemyPos;
	std::vector<EnemyChar*> smallBatsPos;
	Model* slimeModel;
	Model* toadModel;
	Model* batModel;
	Model* bossModel;
	Model* batSmallModel;
	Model* skeletonModel;
public:
	Enemy();
	~Enemy();
	void createSlime(glm::vec3 enemyStartPos);
	void createToad(glm::vec3 enemyStartPos);
	void createGiantBat(glm::vec3 enemyStartPos);
	void createBatSwarm(glm::vec3 enemyStartPos);
	void createSkeleton(glm::vec3 enemyStartPos);
	void createBoss(glm::vec3 enemyStartPos);
	void sortEnemies(glm::vec3 playerPos);
	glm::vec3 getPos()const;
	int getDamage()const;
	void update(float dt, glm::vec3 playerPos);
	void draw(Shader shader);
};
#endif
