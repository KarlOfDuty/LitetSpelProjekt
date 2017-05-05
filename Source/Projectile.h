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
	bool hasCollided;
	sf::Clock timeSinceCollision;
	bool isUsed;
public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	std::string type() const;
	//Own functions
	Projectile();
	~Projectile();
	
	bool isInUse();
	void remove();
	glm::vec2 getPosition();

	void update(float dt, std::vector<Model*> &allObjects);
	void draw(Shader shader);
	void shoot(sf::Window &window, glm::vec2 startPos, Model* arrow);
};

#endif
