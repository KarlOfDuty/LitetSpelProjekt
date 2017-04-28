#include "EnemyChar.h"


EnemyChar::EnemyChar()
{
	this->HP = 10;
	this->damage = 1;
}

EnemyChar::EnemyChar(int HP, Model* model, int damage, glm::vec3 enemyStartPos)
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
	this->enemyModel->setModelMatrix(glm::mat4(
		0.075, 0.0, 0.0, 0.0,
		0.0, 0.075, 0.0, 0.0,
		0.0, 0.0, 0.075, 0.0,
		enemyPos.x, enemyPos.y, enemyPos.z, 1.0
	));
	//this->enemyModelMatrix *= glm::scale(glm::vec3(0.075f, 0.075f, 0.075f));
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
Model* EnemyChar::getModel()
{
	return this->enemyModel;
}

glm::mat4 EnemyChar::getModelMatrix() const
{
	return this->enemyModel->getModelMatrix();
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

bool EnemyChar::checkCollision(std::vector<Model*> &allModels)
{
	int index = -1;
	float minDist = 1000;
	for (int i = 0; i < allModels.size(); i++)
	{
		float distance = glm::length(enemyPos - glm::vec3(allModels[i]->getModelMatrix()[3]));
		if (minDist > distance)
		{
			minDist = distance;
			index = i;
		}
	}
	if (index != -1)
	{
		std::vector<glm::vec2> enemyPoints = this->getModel()->getPoints();
		std::vector<glm::vec2> objectPoints = allModels[index]->getPoints();
		glm::vec2 mtv;
		if (collision::fixCollision(enemyPoints, objectPoints, mtv))
		{
			enemyPos.x += mtv.x;
			enemyPos.y += mtv.y;
			setEnemyPos(enemyPos);
			return true;
		}
	}
	return false;
}

void EnemyChar::update(float dt, glm::vec3 playerPos, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels)
{
	if (glm::length(enemyPos - playerPos) < 25.0f)
	{
		updateThis(dt, playerPos, enemyPos, checkPoint, smallBatsPos, allModels);
		attackPlayer(dt, playerPos, enemyPos);

		checkCollision(allModels);
	}
}

void EnemyChar::draw(Shader shader)
{
	enemyModel->draw(shader);
}
