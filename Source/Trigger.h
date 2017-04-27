#ifndef TRIGGER_H
#define TRIGGER_H
#include <glm\glm.hpp>
#include <vector>
#include "GameObject.h"
#include "Model.h"
#include "Enemy.h"
#include "Player.h"
#include "Light.h"
#include "Projectile.h"
#include "Collision.h"

class Trigger : GameObject
{
private:
	std::vector<glm::vec2> corners;
	std::vector<Model*> activators;
	std::vector<GameObject*> targets;
	//Settings flags
	bool onEnter;
	bool onEnterAll;
	bool onExit;
	bool onExitAll;
	bool whileInside;

	float delay;
	int numberOfActivations;
	int objectsInside;
public:
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	bool update();
	void activate();
	Trigger();
	Trigger(std::vector<glm::vec2> corners, std::vector<Model*> activators, std::vector<GameObject*> targets);
	~Trigger();
};
#endif