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

class Enemy;

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
	int health;
	float velocityX;
	float velocityY;
	bool goingLeft;
	bool goingRight;
	float angle;
	bool isOnGround;
	
	int jumps;
	float movementSpeed;
	float groundPos;
	bool diving;
	enum { CONTROLLER0, CONTROLLER1, CONTROLLER2, CONTROLLER3 };
	std::vector<Model*> debugCubes;
	
	Model* arrow;

	std::vector<Projectile*> allAttackBoxes;
	std::vector<Projectile*> allArrowAttackBoxes;
	std::vector<Projectile*> allAOEAttackBoxes;
	std::vector<Projectile*> allMeleeAttackBoxes;
	
	std::vector<Model*> allStaticModels;

public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	std::string type() const;
	//Own functions
	Player();
	~Player();
	PlayerChar* getCurrentCharacter();
	std::vector<Projectile*> getProjectiles();
	Projectile* getProjectileAt(int nr);
	void swap(int charType);
	bool playerIsDead();
	int getDamage() const;
	int getHealth()const;
	void update(sf::Window &window, float dt, std::vector<Model*> &allModels, std::vector<Enemy*> allEnemies);
	void jump();
	void waterEffect();
	void applyDamage(int appliedDamage);
	void lightAttackPressed(sf::Window &window);
	void lightAttackReleased(sf::Window &window);
	void heavyAttackPressed(sf::Window &window);
	void heavyAttackReleased(sf::Window &window);
	void clearProjectiles();
	void aiming(sf::Window &window, float dt);
	void setPos(glm::vec3 playerPos);
	void draw(Shader shader);
	void collision(std::vector<Model*> &allModels);
	void getPoints(std::vector<glm::vec2> &objectPoints, Model *object, float &radians);
	void setStaticModels(std::vector<Model*> allStaticModels);
	void setCurrentKeyframe(int frame);
	void setAnimationIndex(int index);
	bool getDiving() const;
	void setDiving(bool diving);
	void setHealth(int health);
	void groundCheck();
	//bool firstTimeIdleFrame = true;
};
#endif
