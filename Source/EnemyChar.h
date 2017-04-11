#ifndef ENEMYCHAR_H
#define ENEMYCHAR_H
#include "Model.h"

class EnemyChar
{
private:
	int HP;
	int damage;
	Model enemyModel;
	glm::vec3 enemyPos;
	glm::mat4 enemyModelMatrix;
	bool isOnGround;
	float velocityX;
	float velocityY;
	//Animation animation;
public:

	EnemyChar();
	EnemyChar(int HP, Model model, int damage, glm::vec3 enemyPos);
	virtual ~EnemyChar();
	void setEnemyPos(glm::vec3 position);
	glm::vec3 getEnemyPos() const;
	glm::mat4 getModelMatrix() const;
	void groundCheck();
	void attack();
	void update(float dt, glm::vec3 playerPos);
	void draw(Shader shader);
};
#endif
