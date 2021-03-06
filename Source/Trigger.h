#ifndef TRIGGER_H
#define TRIGGER_H
#include <glm\glm.hpp>
#include <vector>
#include "GameObject.h"
#include "Model.h"
#include "Player.h"
#include "PointLight.h"
#include "Projectile.h"
#include "Collision.h"
extern bool endLevel;
extern bool nextLevel;
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
	//Delay in seconds between repeated activations, 0 for each frame.
	//Any changes in state will be recorded but not cause activations
	float frequency = 0;
	//Maximum allowed activations
	int numberOfActivationsAllowed = -1;
	//If false, all commands will try to execute on all targets
	//If true, each command will execute on their counterpart in the targets vector
	bool accociativeCommands = false;
	//If true, the trigger will use the activator as the target (this also causes perActivator to be true)
	bool accociativeActions = false;
};
class EnemyManager;

class Trigger : public GameObject
{
private:
	std::vector<glm::vec2> corners;
	glm::vec3 pos;
	std::vector<GameObject*> activators;
	std::vector<GameObject*> targets;
	std::vector<std::string> commands;
	TriggerSettings settings;
	//Prevents the trigger from activating before the delay set in the settings
	float timer;
	//Used if there is a maximum number of activations
	int activations;
	//Saved between function calls to be able to tell how many activators entered/exited
	std::vector<bool> objectsInside;
public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	std::string type() const;
	//Own functions
	void move(glm::vec2 distance);
	void setPos(std::vector<glm::vec2> cornerArr);
	void setActivators(std::vector<GameObject*> activators);
	void setTargets(std::vector<GameObject*> targets);
	bool update(float dt);
	void activate(float dt, GameObject* currentActivators, int index);
	void runCommand(int commandID, GameObject* target, float dt);
	//Constructors
	Trigger();
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, std::vector<GameObject*> targets, std::vector<std::string> commands);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, std::vector<GameObject*> targets, std::vector<std::string> commands);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, GameObject* target, std::vector<std::string> commands);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, GameObject* target, std::vector<std::string> commands);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, std::vector<GameObject*> targets, std::string command);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, std::vector<GameObject*> targets, std::string command);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, std::vector<GameObject*> activators, GameObject* target, std::string command);
	Trigger(std::vector<glm::vec2> corners, TriggerSettings settings, GameObject* activator, GameObject* target, std::string command);
	//Destructors
	~Trigger();
};
#endif