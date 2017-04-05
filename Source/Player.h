#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerBird.h"
#include "PlayerShark.h"
#include "PlayerButterfly.h"
#include "Shader.h"
#include <vector>
#include <iostream>

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
	void update(float dt);
	void render(Shader shader);
};
#endif
