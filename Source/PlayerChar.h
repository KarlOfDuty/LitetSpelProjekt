#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "Model.h"

class PlayerChar
{
private:
	int health;
	int damage;
	Model playerModel;
	//Animation animation;
public:
	PlayerChar();
	PlayerChar(int health, Model model);
	virtual ~PlayerChar();
	void applyDamage(int appliedDamage);
	void setHealth(int health);
	int getDamage() const;
	int getHealth()const;
	void attack();
	void draw(Shader shader);
	virtual int getMaxJumps() = 0;
	virtual float getJumpHeight() = 0;
	Model& getModel();
};
#endif