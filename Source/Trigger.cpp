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

}
Trigger::Trigger(std::vector<glm::vec2> corners, std::vector<Model*> activators, std::vector<GameObject*> targets)
{

}
//Destructors
Trigger::~Trigger()
{

}