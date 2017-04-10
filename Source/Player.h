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
	PlayerChar* playerCharacters[3];
	PlayerChar* player;
	void freeMemory();
	glm::mat4 modelMatrix;
	glm::vec3 playerPos;
	Model birdModel;
	Model sharkModel;
	Model butterflyModel;
	float velocityX;
	float velocityY;
	bool isOnGround;
	int jumps;
	float movementSpeed;
	enum {XboxA, XboxB, XboxX, XboxY};
	enum {Controller1, Controller2,Controller3};
	//First dimension size equal to possible number of joysticks,
	//second to number of buttons on each joystick
	bool joystickPressed[8][4] = { false };
	bool keyPressed[120] = { false };
public:
	Player();
	~Player();
	glm::vec3 getPlayerPos();
	void swap(int character);
	void jump(float dt);
	void setPos(glm::vec3 playerPos);
	void update(float dt, sf::Window &window);
	void draw(Shader shader);
};
#endif
