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
bool Trigger::update(float dt)
{
	bool triggered = false;
	//Find how many activators intersect the trigger
	int objectsFound = 0;
	for (int i = 0; i < activators.size(); i++)
	{
		//TODO: Do we need to use both collision methods or just the second one?
		if (collision::testCollision(corners, activators[i]->getPoints()))
		{
			objectsFound++;
		}
		else if (collision::isInside(corners, activators[i]->getPoints()))
		{
			objectsFound++;
		}
	}
	timer += dt;
	//If there is no cooldown active, activate commands according to the settings
	if (timer > settings.delay)
	{

		//onEnter
		if (settings.onEnter && objectsFound > objectsInside)
		{
			for (int i = 0; i < objectsFound - objectsInside; i++)
			{
				activate();
			}
			timer = 0;
			triggered = true;
		}
		//onEnterAll
		if (settings.onEnterAll && activators.size() == objectsFound && objectsFound > 0 && objectsFound > objectsInside)
		{
			if (settings.perActivator)
			{
				for (int i = 0; i < objectsFound; i++)
				{
					activate();
				}
			}
			else
			{
				activate();
			}
			timer = 0;
			triggered = true;
		}
		//onExit
		if (settings.onExit && objectsFound < objectsInside)
		{
			for (int i = 0; i < objectsInside - objectsFound; i++)
			{
				activate();
			}
			timer = 0;
			triggered = true;
		}
		//onExitAll
		if (settings.onEnterAll &&  objectsFound == 0 && objectsFound < objectsInside)
		{
			if (settings.perActivator)
			{
				for (int i = 0; i < objectsFound; i++)
				{
					activate();
				}
			}
			else
			{
				activate();
			}
			timer = 0;
			triggered = true;
		}
		//whileInside
		if (settings.whileInside && objectsFound > 0)
		{
			if (settings.perActivator)
			{
				for (int i = 0; i < objectsFound; i++)
				{
					activate();
				}
			}
			else
			{
				activate();
			}
			timer = 0;
			triggered = true;
		}
		//whileInsideAll
		if (settings.whileAllInside && objectsFound > 0 && objectsFound == activators.size())
		{
			if (settings.perActivator)
			{
				for (int i = 0; i < objectsFound; i++)
				{
					activate();
				}
			}
			else
			{
				activate();
			}
			timer = 0;
			triggered = true;
		}
		//Update the number of activators inside the trigger
		objectsInside = objectsFound;
	}
	else
	{
		//If the trigger is still on cooldown, keep updating the trigger without causing any activations
		objectsInside = objectsFound;
	}
	return triggered;
}
//Activates the trigger
void Trigger::activate()
{
	for (int i = 0; i < commands.size(); i++)
	{
		if (commands[i] == "hellogais")
		{
			std::cout << "Hello gais" << std::endl;
		}
	}
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
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, std::vector<GameObject*> targets, std::vector<std::string> commands)
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
	this->commands = commands;
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, std::vector<GameObject*> targets, std::vector<std::string> commands)
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
	this->commands = commands;
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, GameObject* target, std::vector<std::string> commands)
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
	this->commands = commands;
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, GameObject* target, std::vector<std::string> commands)
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
	this->commands = commands;
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, std::vector<GameObject*> targets, std::string command)
{
	this->corners = corners;
	glm::vec2 middle = glm::vec2(0, 0);
	for (int i = 0; i < corners.size(); i++)
	{
		middle += corners[i];
	}
	middle /= (float)corners.size();
	this->pos = glm::vec3(middle.x, middle.y, 0);
	this->activators = activators;
	this->targets = targets;
	this->settings = settings;
	this->commands.push_back(command);
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, std::vector<GameObject*> targets, std::string command)
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
	this->commands.push_back(command);
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, GameObject* target, std::string command)
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
	this->commands.push_back(command);
}
Trigger::Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, GameObject* target, std::string command)
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
	this->commands.push_back(command);
}
//Destructors
Trigger::~Trigger()
{

}