#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerBird.h"
#include "PlayerShark.h"
#include "PlayerButterfly.h"
#include "Shader.h"
#include "Collision.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include <SFML\Window.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player : public GameObject
{
private:
	PlayerChar* playerCharacters[3];
	PlayerChar* player;
	void freeMemory();
	sf::Clock damageImmunity;
	sf::Clock tpCooldown;
	glm::mat4 modelMatrix;
	glm::mat4 rotationMatrix;
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
	float arrowVelocity;

public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	std::string type() const;
	//Own functions
	Player();
	~Player();
	PlayerChar* getCurrentCharacter();
	void swap(int charType);
	bool playerIsDead();
	int getDamage() const;
	void update(sf::Window &window, float dt, std::vector<Model*> &allModels, std::vector<Enemy*> allEnemies);
	void jump();
	void useLightAttack(sf::Window &window);
	void useHeavyAttack(sf::Window &window);
	void shoot(sf::Window &window);
	void aiming(sf::Window &window, float dt, PlayerBird* bird);
	void setPos(glm::vec3 playerPos);
	void draw(Shader shader);
	void collision(std::vector<Model*> &allModels);
	void getPoints(std::vector<glm::vec2> &objectPoints, Model *object, float &radians);
};
#endif
