#include "Enemy.h"

void Enemy::initiate(int from)
{
	for (int i = from; i < this->CAP; i++)
	{
		this->enemyCharacters[i] = nullptr;
	}
}

void Enemy::expand()
{
	this->CAP += 5;
	EnemyChar* *temp = new EnemyChar*[this->CAP];
	for (int i = 0; i < this->nrOfEnemies; i++)
	{
		temp[i] = this->enemyCharacters[i];
	}
	delete[] this->enemyCharacters;
	this->enemyCharacters = temp;
}

void Enemy::freeMemory()
{
	for (int i = 0; i < this->nrOfEnemies; i++)
	{
		delete this->enemyCharacters[i];
	}
	delete[] this->enemyCharacters;
}

Enemy::Enemy()
{
	this->nrOfEnemies = 0;
	this->CAP = 5;
	this->enemyCharacters = new EnemyChar*[this->CAP];
	this->initiate();
	slimeModel = new Model("models/cube/cube.obj");
}

Enemy::~Enemy()
{
	this->freeMemory();
}

void Enemy::createSlime(glm::vec3 enemyStartPos)
{
	if (this->nrOfEnemies == this->CAP)
	{
		this->expand();
	}
	this->enemyCharacters[this->nrOfEnemies] = new EnemySlime(15, slimeModel, 4, enemyStartPos);
	this->nrOfEnemies++;
}

glm::vec3 Enemy::getPos() const
{
	return enemyCharacters[0]->getPos();
}

int Enemy::getDamage() const
{
	return enemyCharacters[0]->getDamage();
}

void Enemy::update(float dt, glm::vec3 playerPos)
{
		enemyCharacters[0]->update(dt, playerPos);
}

void Enemy::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &enemyCharacters[0]->getModelMatrix()[0][0]);
	enemyCharacters[0]->draw(shader);
}
