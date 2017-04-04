#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerChar.h"
#include <vector>

class Player
{
private:
	std::vector<PlayerChar>* playerCharacters;
public:
	Player();
	~Player();
	void swap();

};
#endif
