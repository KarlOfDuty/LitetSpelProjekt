#ifndef ENEMY_H
#define ENEMY_H
#include "EnemySlime.h"
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
	Model slimeModel;
public:
	Enemy();
	~Enemy();
	void createSlime(glm::vec3 enemyPos);
	glm::vec3 getEnemyPos()const;
	int getDamage()const;
	void update(float dt, glm::vec3 playerPos);
	void draw(Shader shader);
};
#endif
