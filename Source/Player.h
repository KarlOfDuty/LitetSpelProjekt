#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerBird.h"
#include "PlayerShark.h"
#include "PlayerButterfly.h"
#include <vector>

class Player
{
private:
	PlayerChar* *playerCharacters;
	void initiate();
	void freeMemory();
public:
	Player();
	~Player();
	void swap(int charType);

};
#endif
