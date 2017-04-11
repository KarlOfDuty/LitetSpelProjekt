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
	sf::Clock damageImmunity;
	glm::mat4 modelMatrix;
	glm::vec3 playerPos;
	float dx;
	float dy;
	bool isOnGround;
public:
	Player();
	~Player();
	void swap(int charType);
	void groundCheck();
	void setModelMatrix(glm::vec3 playerPos);
	bool playerDead();
	glm::vec3 getPlayerPos() const;
	void update(float dt, glm::vec3 enemyPos, int enemyDamage);
	void draw(Shader shader);
};
#endif
