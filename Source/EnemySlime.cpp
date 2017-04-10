#include "EnemySlime.h"

EnemySlime::EnemySlime(int HP, Model model, int damage) :EnemyChar(HP, model, damage)
{
	HP = 10;
	damage = 4;
	model = model;
}

EnemySlime::~EnemySlime()
{

}

