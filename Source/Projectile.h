#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Model.h"
#include "Collision.h"
#include "SFML\Window.hpp"
#include <glm/glm.hpp>
#include <math.h>

class Projectile
{
private:
	Model* model;
	float rotation;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 direction;
	bool hasCollided;
	sf::Clock timeSinceCollision;
	bool isUsed;
public:
	Projectile();
	~Projectile();
	
	bool isInUse();
	glm::vec2 getPosition();

	void update(float dt, std::vector<Model*> &allObjects);
	void draw(Shader shader);
	void shoot(sf::Window &window, glm::vec2 startPos, Model* arrow);
};

#endif
