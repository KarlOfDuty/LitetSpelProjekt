#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "Model.h"

class PlayerChar
{
private:
	int HP;
	Model playerModel;
	//Animation animation;
public:
	PlayerChar();
	PlayerChar(int HP, Model model);
	virtual ~PlayerChar() = 0;
	PlayerChar(const PlayerChar &originalObject);
	void operator=(const PlayerChar &originalObject);
	void attack();
	void draw(Shader shader);
	virtual int getMaxJumps() = 0;
	virtual float getJumpHeight() = 0;
};
#endif