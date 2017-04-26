#ifndef TRIGGER_H
#define TRIGGER_H
#include <glm\glm.hpp>
#include <vector>
#include "Model.h"
#include "Collision.h"
template<class T> class Trigger
{
private:
	std::vector<glm::vec2> corners;
	std::vector<Model*> activators;
	std::vector<T*> targets;
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
	std::vector<glm::vec2> getCorners() const;
	bool update();
	void activate();
	Trigger<T>();
	Trigger<T>(glm::vec2 topLeft, glm::vec2 topRight, glm::vec2 bottomLeft, glm::vec2 bottomRight);
	Trigger<T>(std::vector<glm::vec2> corners);
	~Trigger();
};
#endif