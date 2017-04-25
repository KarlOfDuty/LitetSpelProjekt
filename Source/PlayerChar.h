#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "Model.h"

class PlayerChar
{
private:
	int HP;
	Model playerModel;
	bool inWater;
	bool diving;
	//Animation animation;
public:
	PlayerChar();
	PlayerChar(int HP, Model model, bool inWater);
	virtual ~PlayerChar();
	void takingDamage(int appliedDamage);
	void setHP(int HP);
	void setSwim(bool swiming);
	void setDiving(bool diving);
	int getHP()const;
	int getDiving()const;
	void attack();
	void draw(Shader shader);
	virtual int getMaxJumps() = 0;
	virtual float getJumpHeight() = 0;
	virtual void waterEffect() = 0;
	Model& getModel();
};
#endif