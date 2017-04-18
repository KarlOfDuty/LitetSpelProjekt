#include "EnemyChar.h"


EnemyChar::EnemyChar()
{
	this->HP = 50;
	this->damage = 10;
}

EnemyChar::EnemyChar(int HP, Model model, int damage, glm::vec3 enemyStartPos)
{
	this->HP = HP;
	this->enemyModel = model;
	this->damage = damage;
	this->enemyPos = enemyStartPos;
	setEnemyPos(enemyPos);
	isOnGround = true;
	this->checkPoint.x = enemyStartPos.x+5;
}

EnemyChar::~EnemyChar()
{

}

void EnemyChar::setEnemyPos(glm::vec3 position)
{
	enemyPos = position;
	this->enemyModelMatrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		enemyPos.x, enemyPos.y, enemyPos.z, 1.0
	);
}

glm::vec3 EnemyChar::getEnemyPos() const
{
	return enemyPos;
}

int EnemyChar::getDamage() const
{
	return damage;
}

glm::mat4 EnemyChar::getModelMatrix() const
{
	return enemyModelMatrix;
}

void EnemyChar::groundCheck()
{
	if (this->enemyPos.y > 0.0f)
	{
		isOnGround = false;
	}
}

void EnemyChar::update(float dt, glm::vec3 playerPos)
{
	updateThis(dt, playerPos, enemyPos);
	attackPlayer(dt, playerPos, enemyPos);
}

void EnemyChar::draw(Shader shader)
{
	enemyModel.draw(shader);
}
