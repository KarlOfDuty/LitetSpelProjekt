#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "Model.h"

class PlayerChar
{
private:
	int HP;
	int damage;
	Model playerModel;
	//Animation animation;
public:
	PlayerChar();
	PlayerChar(int HP, Model model);
	virtual ~PlayerChar();
	void takingDamage(int appliedDamage);
	void setHP(int HP);
	int getDamage() const;
	int getHP()const;
	void attack();
	void draw(Shader shader);
	virtual int getMaxJumps() = 0;
	virtual float getJumpHeight() = 0;
	Model& getModel();
};
#endif