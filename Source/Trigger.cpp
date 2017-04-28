#include "Trigger.h"
std::vector<glm::vec2> Trigger::getPoints()
{
	return corners;
}
glm::vec3 Trigger::getPos() const
{
	return pos;
}
//Update function, checks for activators and runs activate(), also returns true if any activations were made
bool Trigger::update()
{
	bool triggered = false;
	//Find how many activators intersect this one
	int numOfModels = 0;
	for (int i = 0; i < activators.size(); i++)
	{
		if (collision::testCollision(corners, activators[i]->getPoints()))
		{
			numOfModels++;
		}
	}
	//Check settings flags - objectsInside is how many objects were inside last frame, 
	//used to make sure triggers only fire once if that is it's setting
	if (settings.onEnter && numOfModels > objectsInside)
	{
		for (int i = 0; i < numOfModels - objectsInside; i++)
		{
			activate();
		}
		triggered = true;
	}
	if (settings.onEnterAll && activators.size() == numOfModels && numOfModels > 0 && numOfModels > objectsInside)
	{
		if (settings.perActivator)
		{
			for (int i = 0; i < numOfModels; i++)
			{
				activate();
			}
		}
		else
		{
			activate();
		}
		triggered = true;
	}
	if (settings.onExit && numOfModels < objectsInside)
	{
		for (int i = 0; i < objectsInside - numOfModels; i++)
		{
			activate();
		}
		triggered = true;
	}
	if (settings.onEnterAll &&  numOfModels == 0 && numOfModels < objectsInside)
	{
		if (settings.perActivator)
		{
			for (int i = 0; i < numOfModels; i++)
			{
				activate();
			}
		}
		else
		{
			activate();
		}
		triggered = true;
	}
	if (settings.whileInside && numOfModels > 0)
	{
		if (settings.perActivator)
		{
			for (int i = 0; i < numOfModels; i++)
			{
				activate();
			}
		}
		else
		{
			activate();
		}
		triggered = true;
	}
	if (settings.whileAllInside && numOfModels > 0 && numOfModels == activators.size())
	{
		if (settings.perActivator)
		{
			for (int i = 0; i < numOfModels; i++)
			{
				activate();
			}
		}
		else
		{
			activate();
		}
		triggered = true;
	}
	objectsInside = numOfModels;
	return triggered;
}
//Activates the trigger
void Trigger::activate()
{
	std::cout << "Hello gais" << std::endl;
}
//Constructors
Trigger::Trigger()
{
	this->corners = std::vector<glm::vec2>();
	this->pos = glm::vec3(0,0,0);
	this->activators = std::vector<GameObject*>();
	this->targets = std::vector<GameObject*>();
	this->settings = TriggerSettings();
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, std::vector<GameObject*> targets)
{
	this->corners = corners;
	glm::vec2 middle = glm::vec2(0,0);
	for (int i = 0; i < corners.size(); i++)
	{
		middle += corners[i];
	}
	middle /= (float)corners.size();
	this->pos = glm::vec3(middle.x, middle.y, 0);
	this->activators = activators;
	this->targets = targets;
	this->settings = settings;
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, std::vector<GameObject*> targets)
{
	this->corners = corners;
	glm::vec2 middle = glm::vec2(0, 0);
	for (int i = 0; i < corners.size(); i++)
	{
		middle += corners[i];
	}
	middle /= (float)corners.size();
	this->pos = glm::vec3(middle.x, middle.y, 0);
	this->activators.push_back(activator);
	this->targets = targets;
	this->settings = settings;
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, GameObject* target)
{
	this->corners = corners;
	glm::vec2 middle = glm::vec2(0, 0);
	for (int i = 0; i < corners.size(); i++)
	{
		middle += corners[i];
	}
	middle /= (float)corners.size();
	this->activators = activators;
	this->targets.push_back(target);
	this->settings = settings;
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, GameObject* target)
{
	this->corners = corners;
	glm::vec2 middle = glm::vec2(0, 0);
	for (int i = 0; i < corners.size(); i++)
	{
		middle += corners[i];
	}
	middle /= (float)corners.size();
	this->activators.push_back(activator);
	this->targets.push_back(target);
	this->settings = settings;
}
//Destructors
Trigger::~Trigger()
{

}