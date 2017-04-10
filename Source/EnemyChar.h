#ifndef ENEMYCHAR_H
#define ENEMYCHAR_H
#include "Model.h"

class EnemyChar
{
public:
	int HP;
	int damage;
	Model enemyModel;
	//Animation animation;
private:
	EnemyChar(int HP, Model enemyModel, int damage);
	virtual ~EnemyChar();
	void attack();
	void draw(Shader shader);
};
#endif
