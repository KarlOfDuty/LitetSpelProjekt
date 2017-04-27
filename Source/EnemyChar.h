#ifndef ENEMYCHAR_H
#define ENEMYCHAR_H
#include "Model.h"

class EnemyChar : GameObject
{
private:
	int HP;
	int damage;
	Model *model;
	glm::vec3 enemyPos;
	glm::mat4 enemyModelMatrix;
	glm::vec3 checkPoint;
	//Animation animation;
public:
	//Variables
	float velocityX;
	float velocityY;
	bool isOnGround;

	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	//Own functions
	EnemyChar();
	EnemyChar(int HP, Model *model, int damage, glm::vec3 enemyPos);
	virtual ~EnemyChar();
	void setPos(glm::vec3 position);
	int getDamage()const;
	glm::mat4 getModelMatrix() const;
	void groundCheck();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos) = 0;
	void update(float dt, glm::vec3 playerPos);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint) = 0;
	void draw(Shader shader);
};
#endif
