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
	Model playerModel;
	//Animation animation;
	bool mayJumpAgain;
public:
	PlayerChar();
	PlayerChar(int HP, Model model);
	virtual ~PlayerChar();
	PlayerChar(const PlayerChar &originalObject);
	void operator=(const PlayerChar &originalObject);
	bool getJumpAvailable();
	void setJumpAvailable(bool available);
	void attack();
	void draw(Shader shader);
};
#endif