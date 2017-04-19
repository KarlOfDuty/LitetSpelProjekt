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
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player
{
private:
	PlayerChar* playerCharacters[3];
	PlayerChar* player;
	void freeMemory();
	sf::Clock damageImmunity;
	glm::mat4 modelMatrix;
	glm::mat4 rotationMatrix;
	glm::vec3 playerPos;
	float velocityX;
	float velocityY;
	bool goingLeft;
	bool goingRight;
	float angle;
	bool isOnGround;
	int jumps;
	float movementSpeed;
	float groundPos;
	enum { CONTROLLER0, CONTROLLER1, CONTROLLER2, CONTROLLER3 };
	std::vector<Model*> debugCubes;
public:
	Player();
	~Player();
	void swap(int charType);
	void groundCheck();
	bool playerDead();
	glm::vec3 getPlayerPos() const;
	void update(float dt, std::vector<Model*> &allModels, glm::vec3 enemyPos, int enemyDamage);
	glm::vec3 getPlayerPos();
	void jump();
	void setPos(glm::vec3 playerPos);
	void draw(Shader shader);
	void fixCollision(std::vector<Model*> &allModels);
	bool checkCollision(Model* object, glm::vec2 &mtv);
	std::vector<glm::vec2> getAxis(std::vector<glm::vec2> points1, std::vector<glm::vec2> points2);
};
#endif
