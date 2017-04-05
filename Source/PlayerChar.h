#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "Model.h"
#include "PlayerBird.h"
#include "PlayerShark.h"
#include "PlayerButterfly.h"

class PlayerChar
{
private:
	int HP;
	//Animation animation;
public:
	PlayerChar();
	PlayerChar(int HP);
	virtual ~PlayerChar();
	PlayerChar(const PlayerChar &originalObject);
	void operator=(const PlayerChar &originalObject);
	virtual Model getModelNow()const = 0;
	virtual void jump() = 0;
	virtual void moveLeft() = 0;
	virtual void moveRight() = 0;
	virtual void attack() = 0;
	
};
#endif