#include "EnemyChar.h"


EnemyChar::EnemyChar()
{
	this->HP = 50;
	this->damage = 10;
}

EnemyChar::EnemyChar(int HP, Model model, int damage)
{
	this->HP = HP;
	this->enemyModel = model;
	this->damage = damage;
}

EnemyChar::~EnemyChar()
{

}

void EnemyChar::attack()
{

}

void EnemyChar::draw(Shader shader)
{
	enemyModel.draw(shader);
}
