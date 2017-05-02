#ifndef ENEMYCHAR_H
#define ENEMYCHAR_H
#include "Model.h"
#include "Collision.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML\Window.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>  
#include <random>

class EnemyChar : GameObject
{
private:
	float HP;
	int damage;
	sf::Clock damageImmunity;
	Model *model;
	glm::vec3 enemyPos;
	//glm::mat4 enemyModelMatrix;
	glm::vec3 checkPoint;
	//Animation animation;
public:
	//Variables
	float velocityX;
	float velocityY;
	bool isOnGround;
	bool playerSeen;

	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	virtual std::string type() const;
	//Own functions
	EnemyChar();
	EnemyChar(int HP, Model* model, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyChar();
	void setPos(glm::vec3 position);
	void setHP(int HP);
	glm::vec3 getPos() const;
	int getDamage()const;
	int getHP() const;
	Model* getModel();
	glm::mat4 getModelMatrix() const;
	void takingDamage(int appliedDamage);
	void groundCheck();
	bool checkCollision(std::vector<Model*> &allModels);
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos) = 0;
	void update(float dt, glm::vec3 playerPos, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels) = 0;
	void draw(Shader shader);
};
#endif
