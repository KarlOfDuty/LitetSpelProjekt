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
	sf::Clock deltaClock;
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
	enum { CONTROLLER0, CONTROLLER1, CONTROLLER2, CONTROLLER3 };
public:
	Player();
	~Player();
	glm::vec3 getPlayerPos();
	void swap(int character);
	void jump();
	void setPos(glm::vec3 playerPos);
	void update(float dt, sf::Window &window);
	void draw(Shader shader);
};
#endif
