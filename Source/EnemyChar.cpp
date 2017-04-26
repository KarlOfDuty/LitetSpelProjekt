#include "EnemyChar.h"


EnemyChar::EnemyChar()
{
	this->HP = 10;
	this->damage = 1;
}

EnemyChar::EnemyChar(int HP, Model model, int damage, glm::vec3 enemyStartPos)
{
	this->HP = HP;
	this->enemyModel = model;
	this->damage = damage;
	this->enemyPos = enemyStartPos;
	setEnemyPos(enemyPos);
	isOnGround = true;
	playerSeen = false;
	this->checkPoint.x = enemyStartPos.x;
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
	this->enemyModelMatrix *= glm::scale(glm::vec3(0.075f, 0.075f, 0.075f));
}

void EnemyChar::setHP(int HP)
{
	this->HP = HP;
}

glm::vec3 EnemyChar::getEnemyPos() const
{
	return enemyPos;
}

int EnemyChar::getDamage() const
{
	return damage;
}

int EnemyChar::getHP() const
{
	return this->HP;
}

glm::mat4 EnemyChar::getModelMatrix() const
{
	return enemyModelMatrix;
}

void EnemyChar::takingDamage(int appliedDamage)
{
	if (this->damageImmunity.getElapsedTime().asSeconds() >= 0.5)
	{
		this->HP -= appliedDamage;
		this->damageImmunity.restart();
	}
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
	enemyModel.draw(shader);
}
