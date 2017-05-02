#include "EnemyChar.h"


EnemyChar::EnemyChar()
{
	this->HP = 10;
	this->damage = 1;
}


EnemyChar::EnemyChar(int HP, Model* model, int damage, glm::vec3 enemyStartPos)
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
	this->model->setModelMatrix(glm::mat4(
		0.075, 0.0, 0.0, 0.0,
		0.0, 0.075, 0.0, 0.0,
		0.0, 0.0, 0.075, 0.0,
		enemyPos.x, enemyPos.y, enemyPos.z, 1.0
	));
}

void EnemyChar::setHP(int HP)
{
	this->HP = HP;
}

glm::vec3 EnemyChar::getPos() const
{
	return enemyPos;
}

std::string EnemyChar::type() const
{
	return "Enemy";
}

std::vector<glm::vec2> EnemyChar::getPoints()
{
	return model->getPoints();
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
	return this->model;
}

glm::mat4 EnemyChar::getModelMatrix() const
{
	return this->model->getModelMatrix();
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
		if (collision::testCollision(enemyPoints, objectPoints, mtv))
		{
			enemyPos.x += mtv.x;
			enemyPos.y += mtv.y;
			setPos(enemyPos);
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

	}
}

void EnemyChar::draw(Shader shader)
{
	model->draw(shader);
}
