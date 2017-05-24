#ifndef ENEMY_H
#define ENEMY_H
#include "Model.h"
#include "Collision.h"
#include "Projectile.h"
#include "Trigger.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML\Window.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>  
#include <random>

class Trigger;

class Player;

class Enemy : public GameObject
{
private:
	float health;
	int damage;
	int immunityTime;
	sf::Clock damageImmunity;
	Model *model;
	glm::vec3 pos;
	glm::vec3 checkPoint;
	//Animation animation;
public:
	//Variables
	float velocityX;
	float velocityY;
	bool isOnGround;
	bool playerSeen;
	bool bossImmunity;
	glm::vec2 collidedFrom;
	float radians;
	float groundPos;
	float angle;
	bool rotation;
	glm::vec3 scaleFactor;
	bool bossKill;

	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	virtual std::string type() const;
	//Own functions
	Enemy();
	Enemy(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor);
	virtual ~Enemy();
	void setPos(glm::vec3 position);
	void setHealth(int health);
	int getDamage()const;
	int getHealth() const;
	Model* getModel();
	void rotateModel(float direction);
	void applyDamage(int appliedDamage);
	void groundCheck();
	void setBossImmunity(bool isImmune);
	bool collision(std::vector<Model*> &allModels);
	bool collisionWithPlayer(Player* player);
	bool getBossKill()const;
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 pos) = 0;
	void update(float dt, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player);
	virtual void updateThis(float dt, glm::vec3 pos, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player) = 0;
	void draw(Shader shader);
};
#endif
