#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerBird.h"
#include "PlayerShark.h"
#include "PlayerButterfly.h"
#include "Shader.h"
#include "Collision.h"
#include "Projectile.h"
#include "GameObject.h"
#include <SFML\Window.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player : GameObject
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
	Model* arrow;
	std::vector<Projectile*> arrows;
public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	//Own functions
	Player();
	~Player();
	void swap(int charType);
	bool playerIsDead();
	glm::vec3 getPlayerPos() const;
	glm::vec3 getActualPlayerPos() const;
	void update(sf::Window &window, float dt, std::vector<Model*> &allModels, glm::vec3 enemyPos, int enemyDamage);
	void jump();
	void shoot(sf::Window &window);
	void setPos(glm::vec3 playerPos);
	void setActualPos(glm::vec3 playerPos);
	void draw(Shader shader);
	void testCollision(std::vector<Model*> &allModels);
	std::vector<glm::vec2> getPlayerPoints();
	void getPoints(std::vector<glm::vec2> &objectPoints, Model *object, float &radians);
};
#endif
