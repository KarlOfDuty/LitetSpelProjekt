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
	float rotation;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 direction;
	glm::vec2 retardation;
	glm::vec3 scale;
	bool isRotating;
	bool hasCollided;
	sf::Clock timeSinceCollision;
	bool isUsed;
	bool isAoe;
public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	std::string type() const;
	//Own functions
	Projectile();
	~Projectile();
	
	bool isInUse();
	void disableArrow();
	glm::vec2 getPosition();

	void update(float dt, std::vector<Model*> &allObjects);
	void draw(Shader shader);
	void shoot(Model* projectileModel, glm::vec2 startPos, glm::vec2 projectileDirection, glm::vec2 projectileRetardation, float projectileVelocity, glm::vec3 projectileScale, bool shouldRotate = true);
	void aoe(Model* projectileModel, glm::vec2 startPos, glm::vec2 projectileDirection, glm::vec2 projectileRetardation, float projectileVelocity, glm::vec3 projectileScale);
	void collision(std::vector<Model*> &allObjects);
};

#endif
