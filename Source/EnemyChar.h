#ifndef ENEMYCHAR_H
#define ENEMYCHAR_H
#include "Model.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML\Window.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>  

class EnemyChar
{
private:
	int HP;
	int damage;
	Model enemyModel;
	glm::vec3 enemyPos;
	glm::mat4 enemyModelMatrix;
	glm::vec3 checkPoint;
	//Animation animation;
public:
	//Variables
	float velocityX;
	float velocityY;
	bool isOnGround;
	bool playerSeen;

	//Functions
	EnemyChar();
	EnemyChar(int HP, Model model, int damage, glm::vec3 enemyPos);
	virtual ~EnemyChar();
	void setEnemyPos(glm::vec3 position);
	glm::vec3 getEnemyPos() const;
	int getDamage()const;
	glm::mat4 getModelMatrix() const;
	void groundCheck();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos) = 0;
	void update(float dt, glm::vec3 playerPos, std::vector<EnemyChar*> smallBatsPos);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos) = 0;
	void draw(Shader shader);
};
#endif
