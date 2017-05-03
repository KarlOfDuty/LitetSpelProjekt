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

class Enemy : GameObject
{
private:
	float health;
	int damage;
	sf::Clock damageImmunity;
	Model *model;
	glm::vec3 pos;
	//glm::mat4 enemyModelMatrix;
	glm::vec3 checkPoint;
	//Animation animation;
public:
	//Variables
	float velocityX;
	float velocityY;
	bool isOnGround;
	bool playerSeen;
	glm::vec2 collidedFrom;

	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	virtual std::string type() const;
	//Own functions
	Enemy();
	Enemy(int health, Model* model, int damage, glm::vec3 enemyStartPos);
	virtual ~Enemy();
	void setPos(glm::vec3 position);
	void setHealth(int health);
	int getDamage()const;
	int getHealth() const;
	Model* getModel();
	void applyDamage(int appliedDamage);
	void groundCheck();
	bool collision(std::vector<Model*> &allModels);
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 pos) = 0;
	void update(float dt, glm::vec3 playerPos, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 pos, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels) = 0;
	void draw(Shader shader);
};
#endif
