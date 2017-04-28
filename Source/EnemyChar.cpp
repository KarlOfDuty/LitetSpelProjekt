#include "EnemyChar.h"


EnemyChar::EnemyChar()
{
	this->HP = 10;
	this->damage = 1;
}

EnemyChar::EnemyChar(int HP, Model *model, int damage, glm::vec3 enemyStartPos)
{
	this->HP = HP;
	this->model = model;
	this->damage = damage;
	this->enemyPos = enemyStartPos;
	setPos(enemyPos);
	isOnGround = true;
	playerSeen = false;
	this->checkPoint.x = enemyStartPos.x;
}

EnemyChar::~EnemyChar()
{

}

void EnemyChar::setPos(glm::vec3 position)
{
	enemyPos = position;
	this->enemyModelMatrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		enemyPos.x, enemyPos.y, enemyPos.z, 1.0
	);
	this->enemyModelMatrix *= glm::scale(glm::vec3(0.075f, 0.075f, 0.075f));
}

glm::vec3 EnemyChar::getPos() const
{
	return enemyPos;
}

std::vector<glm::vec2> EnemyChar::getPoints()
{
	return model->getPoints();
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

void EnemyChar::update(float dt, glm::vec3 playerPos, std::vector<EnemyChar*> smallBatsPos)
{
	updateThis(dt, playerPos, enemyPos, checkPoint, smallBatsPos);
	attackPlayer(dt, playerPos, enemyPos);
}

void EnemyChar::draw(Shader shader)
{
	model->draw(shader);
}
