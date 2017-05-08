#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "Model.h"

class PlayerChar
{
private:
	int health;
	int damage;
	Model playerModel;
	bool inWater;
	
	//Animation animation;
public:
	PlayerChar();
	PlayerChar(int health, Model model, bool inWater);
	virtual ~PlayerChar();
	void applyDamage(int appliedDamage);
	void setHealth(int health);
	void setSwim(bool swiming);
	int getDamage() const;
	int getHealth()const;
	void attack();
	void draw(Shader shader);
	virtual int getMaxJumps() = 0;
	virtual float getJumpHeight() = 0;
	virtual void waterEffect() = 0;
	Model& getModel();
};
#endif