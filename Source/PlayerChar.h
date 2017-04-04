#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "PlayerBird.h"
#include "PlayerShark.h"
#include "PlayerButterfly.h"

class PlayerChar
{
private:
	int HP;
	//Model model;
	//Animation animation;
public:
	PlayerChar(int HP);
	~PlayerChar();
	virtual void jump() = 0;
	virtual void moveLeft() = 0;
	virtual void moveRight() = 0;
	virtual void attack() = 0;
	
};
#endif