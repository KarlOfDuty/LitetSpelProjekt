#include "Trigger.h"
std::vector<glm::vec2> Trigger::getPoints()
{
	return corners;
}
glm::vec3 Trigger::getPos() const
{
	return glm::vec3();
}
bool Trigger::update()
{
	bool triggered = false;
	//Find how many models intersect this one
	int numOfModels = 0;
	for (int i = 0; i < activators.size(); i++)
	{
		if (collision::testCollision(corners, activators[i]->getPoints()))
		{
			numOfModels++;
		}
	}
	if (onEnter && numOfModels > objectsInside)
	{
		for (int i = 0; i < numOfModels - objectsInside; i++)
		{
			activate();
		}
		triggered = true;
	}
	if (onEnterAll && activators.size() == numOfModels && numOfModels > 0)
	{
		activate();
		triggered = true;
	}
	if (whileInside && objectsInside)
	{
		for (int i = 0; i < numOfModels; i++)
		{
			activate();
		}
		triggered = true;
	}
	objectsInside = numberOfActivations;
	return triggered;
}
void Trigger::activate()
{
	std::cout << "Hello gais" << std::endl;
}
//Constructors
Trigger::Trigger()
{
	this->corners = std::vector<glm::vec2>();
	this->activators = std::vector<GameObject*>();
	this->targets = std::vector<GameObject*>();
}
Trigger::Trigger(std::vector<glm::vec2> corners, std::vector<GameObject*> activators, std::vector<GameObject*> targets)
{
	this->corners = corners;
	this->activators = activators;
	this->targets = targets;
}
Trigger::Trigger(std::vector<glm::vec2> corners, GameObject* activator, std::vector<GameObject*> targets)
{
	this->corners = corners;
	this->activators.push_back(activator);
	this->targets = targets;
}
Trigger::Trigger(std::vector<glm::vec2> corners, std::vector<GameObject*> activators, GameObject* target)
{
	this->corners = corners;
	this->activators = activators;
	this->targets.push_back(target);
}
Trigger::Trigger(std::vector<glm::vec2> corners, GameObject* activator, GameObject* target)
{
	this->corners = corners;
	this->activators.push_back(activator);
	this->targets.push_back(target);
}
//Destructors
Trigger::~Trigger()
{

}