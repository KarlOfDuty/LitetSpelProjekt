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
struct TriggerSettings
{
	//Activates once an activator enters the trigger
	bool onEnter = false;
	//Activates once when all activators enter the trigger
	bool onEnterAll = false;
	//Activates every time an activator leaves the trigger
	bool onExit = false;
	//Activates once when all activators have left the trigger
	bool onExitAll = false;
	//Activates each frame an activator is inside the trigger
	bool whileInside = false;
	//Activates each frame while all activators are inside of the trigger
	bool whileAllInside = false;
	//Modifies previous flags to activate once for each activator inside the trigger 
	//Works with onEnterAll, onExitAll, whileInside and whileAllInside
	bool perActivator = false;
	//Delay in ms between repeated activations, 0 for each frame
	float delay = 0;
	//Maximum allowed activations, set to 0 for infinite
	int numberOfActivationsAllowed = 0;
	//Decides what actions will be taken on the targets
	std::vector<std::string> actions;
};
class Trigger : public GameObject
{
private:
	std::vector<glm::vec2> corners;
	glm::vec3 pos;
	std::vector<GameObject*> activators;
	std::vector<GameObject*> targets;
	TriggerSettings settings;
	float delay;
	int activations;
	int objectsInside;
public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	//Own functions
	bool update(float dt);
	void activate();
	//Constructors
	Trigger();
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, std::vector<GameObject*> targets);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, std::vector<GameObject*> targets);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, GameObject* target);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, GameObject* target);
	//Destructors
	~Trigger();
};
#endif