#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Model.h"
#include "Collision.h"
#include "SFML\Window.hpp"
#include <glm/glm.hpp>
#include <math.h>

class Projectile : public GameObject
{
private:
	Model* model;
	int damage;
	float rotation;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 direction;
	glm::vec2 retardation;
	glm::vec3 scale;
	bool isRotating;
	bool hasCollided;
	bool deleteOnImpact;
	bool isUsed;
	bool isProjectile;
	bool isAoe;
	bool isMelee;
	bool enemyMeleeAttack;
	sf::Clock timeSinceCollision;
public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	std::string type() const;
	//Own functions
	Projectile();
	~Projectile();
	bool isInUse();
	bool isProjectileAttack();
	bool isAoeAttack();
	bool isMeleeAttack();
	bool isCollidingWithWorld();
	void disableArrow();
	glm::vec2 getPosition();
	int getDamage();
	void update(float dt, std::vector<Model*> &allObjects, glm::vec2 playerPos);
	void draw(Shader shader);
	void shoot(Model* projectileModel, int projectileDamage, glm::vec2 startPos, glm::vec2 projectileDirection, glm::vec2 projectileRetardation, float projectileVelocity, glm::vec3 projectileScale, bool shouldRotate = true, bool deleteOnImpact = false);
	void aoe(Model* projectileModel, int projectileDamage, glm::vec2 startPos, glm::vec2 projectileDirection, float projectileVelocity, glm::vec3 projectileScale);
	void melee(Model* projectileModel, int projectileDamage, glm::vec2 startPos, glm::vec2 projectileDirection, float projectileVelocity, glm::vec3 projectileScale);
	void enemyMelee(Model* projectileModel, int projectileDamage, glm::vec2 startPos, glm::vec2 projectileDirection, float projectileVelocity, glm::vec3 projectileScale);
	void collision(std::vector<Model*> &allObjects);
};

#endif
