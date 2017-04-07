#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerBird.h"
#include "PlayerShark.h"
#include "PlayerButterfly.h"
#include "Shader.h"
#include <SFML\Window.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <iostream>

class Player
{
private:
	PlayerChar* *playerCharacters;
	void initiate();
	void freeMemory();
	glm::mat4 modelMatrix;
	glm::vec3 playerPos;
	Model birdModel;
	Model sharkModel;
	Model butterflyModel;
	float dx;
	float dy;
	bool isOnGround;
public:
	Player();
	~Player();
	glm::vec3 getPlayerPos();
	void swap(int charType);
	void groundCheck();
	void setModelMatrix(glm::vec3 playerPos);
	void update(float dt, int &jumpPress, bool &keyReleased);
	void draw(Shader shader);
};
#endif
