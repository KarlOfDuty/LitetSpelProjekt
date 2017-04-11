#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "Model.h"

class PlayerChar
{
private:
	int HP;
	Model playerModel;
	//Animation animation;
	bool mayJumpAgain;
public:
	PlayerChar();
	PlayerChar(int HP, Model model);
	virtual ~PlayerChar();
	bool getJumpAvailable();
	void setJumpAvailable(bool available);
	void takingDamage(int appliedDamage);
	void setHP(int HP);
	int getHP()const;
	void attack();
	void draw(Shader shader);
};
#endif