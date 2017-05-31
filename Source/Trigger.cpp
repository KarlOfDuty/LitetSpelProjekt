#include "Trigger.h"
#include "EnemyManager.h"

std::vector<glm::vec2> Trigger::getPoints()
{
	return corners;
}
void Trigger::move(glm::vec2 distance)
{
	for (int i = 0; i < corners.size(); i++)
	{
		corners[i].x += distance.x;
		corners[i].y += distance.y;
	}
}
void Trigger::setPos(std::vector<glm::vec2> cornerArr)
{
	for (int i = 0; i < corners.size(); i++)
	{
		corners[i] = cornerArr[i];
	}
}

void Trigger::setActivators(std::vector<GameObject*> activators)
{
	this->activators = activators;
}
void Trigger::setTargets(std::vector<GameObject*> targets)
{
	this->targets = targets;
}

glm::vec3 Trigger::getPos() const
{
	return pos;
}
std::string Trigger::type() const
{
	return "Trigger";
}
//Update function, checks for activators and runs activate(), also returns true if any activations were made
bool Trigger::update(float dt)
{
	bool triggered = false;
	if (settings.numberOfActivationsAllowed == -1 || settings.numberOfActivationsAllowed > 0)
	{
		//Find how many activators intersect the trigger
		std::vector<bool> objectsFound = std::vector<bool>(activators.size());
		int numObjectsFound = 0;
		for (int i = 0; i < activators.size(); i++)
		{
			if (collision::collision(corners, activators[i]->getPoints()))
			{
				objectsFound[i] = true;
				numObjectsFound++;
			}
			else if (collision::isInside(corners, activators[i]->getPoints()))
			{
				objectsFound[i] = true;
				numObjectsFound++;
			}
			else
			{
				objectsFound[i] = false;
			}
		}
		timer += dt;

		//If there is no cooldown active, activate commands according to the settings
		if (timer > settings.frequency)
		{
			//onEnter and onExit
			for (int i = 0; i < activators.size(); i++)
			{
				if (objectsFound[i] && !objectsInside[i] && settings.onEnter)
				{
					activate(dt, activators[i], i);
					triggered = true;
				}
				else if (!objectsFound[i] && objectsInside[i] && settings.onExit)
				{
					activate(dt, activators[i], i);
					triggered = true;
				}
			}
			//onEnterAll
			if (settings.onEnterAll && numObjectsFound > 0 && numObjectsFound == activators.size())
			{
				bool found = false;
				for (int i = 0; i < objectsInside.size() && !found; i++)
				{
					if (!objectsInside[i])
					{
						found = true;
					}
				}
				if (found)
				{
					if (settings.perActivator || settings.accociativeActions)
					{
						for (int i = 0; i < activators.size(); i++)
						{
							activate(dt, activators[i], i);
						}
					}
					else
					{
						activate(dt, activators[0], 0);
					}
					triggered = true;
				}

			}
			//onExitAll
			if (settings.onExitAll && numObjectsFound == 0)
			{
				bool found = false;
				for (int i = 0; i < objectsInside.size() && !found; i++)
				{
					if (objectsInside[i])
					{
						found = true;
					}
				}
				if (found)
				{
					if (settings.perActivator || settings.accociativeActions)
					{
						for (int i = 0; i < activators.size(); i++)
						{
							activate(dt, activators[i], i);
						}
					}
					else
					{
						activate(dt, activators[0], 0);
					}
					triggered = true;
				}

			}
			//whileInside
			if (settings.whileInside && numObjectsFound > 0)
			{
				if (settings.perActivator || settings.accociativeActions)
				{
					for (int i = 0; i < objectsFound.size(); i++)
					{
						if (objectsFound[i])
						{
							activate(dt, activators[i], i);
						}
					}
				}
				else
				{
					activate(dt, activators[0], 0);
				}
				triggered = true;
			}
			//whileAllInside
			if (settings.whileAllInside && numObjectsFound > 0 && numObjectsFound == activators.size())
			{
				if (settings.perActivator || settings.accociativeActions)
				{
					for (int i = 0; i < objectsFound.size(); i++)
					{
						if (objectsFound[i])
						{
							activate(dt, activators[i], i);
						}
					}
				}
				else
				{
					activate(dt, activators[0], 0);
				}
				triggered = true;
			}
			//Update which activators are inside the trigger
			objectsInside = objectsFound;
		}
		else
		{
			//If the trigger is still on cooldown, keep updating the trigger without causing any activations
			objectsInside = objectsFound;
		}
	}
	return triggered;
}
//Activates the trigger
void Trigger::activate(float dt, GameObject* activator, int index)
{
	if (settings.accociativeActions)
	{
		//Target only the objects that activated the trigger
		if (settings.accociativeCommands)
		{
			if (index < commands.size())
			{
				runCommand(index,activator, dt);
				timer = 0;
				//numberOfActivationsAllowed
				if (settings.numberOfActivationsAllowed != -1)
				{
					settings.numberOfActivationsAllowed--;
				}
			}
		}
		else
		{
			//Activates each command on each target
			for (int i = 0; i < commands.size(); i++)
			{
				runCommand(i, activator, dt);
				timer = 0;
				//numberOfActivationsAllowed
				if (settings.numberOfActivationsAllowed != -1)
				{
					settings.numberOfActivationsAllowed--;
				}
			}
		}
	}
	else
	{
		//Target the objects in the targets vector
		if (settings.accociativeCommands)
		{
			//Activate each command on their counterpart in targets
			for (int i = 0; i < commands.size() && i < targets.size(); i++)
			{
				runCommand(i, targets[i], dt);
				timer = 0;
				//numberOfActivationsAllowed
				if (settings.numberOfActivationsAllowed != -1)
				{
					settings.numberOfActivationsAllowed--;
				}
			}
		}
		else
		{
			//Activates each command on each target
			for (int i = 0; i < commands.size(); i++)
			{
				for (int j = 0; j < targets.size(); j++)
				{
					runCommand(i, targets[j], dt);
					timer = 0;
					//numberOfActivationsAllowed
					if (settings.numberOfActivationsAllowed != -1)
					{
						settings.numberOfActivationsAllowed--;
					}
				}
			}
		}
	}
}
void Trigger::runCommand(int commandID, GameObject* target, float dt)
{
	//Add new commands here
	if (commands[commandID] == "hellogais")
	{
		std::cout << "Hello gais" << std::endl;
	}
	else if (commands[commandID] == "water" && target->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(target);
		player->setDiving(!player->getDiving());
	}
	else if (commands[commandID] == "water" && target->type() == "Enemy")
	{
		Enemy* enemy = dynamic_cast<Enemy*>(target);
		enemy->setHealth(0);
	}
	else if (commands[commandID] == "nextLevel")
	{
		nextLevel = true;
	}
	else if (commands[commandID] == "fire" && target->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(target);
		player->applyDamage(1);
	}
	else if (commands[commandID] == "spikes" && target->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(target);
		player->applyDamage(10);
	}
	else if (commands[commandID] == "kill" && target->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(target);
		player->setHealth(0);
	}
	else if (commands[commandID] == "kill" && target->type() == "Enemy")
	{
		Enemy* enemy = dynamic_cast<Enemy*>(target);
		enemy->setHealth(0);
	}
	else if (commands[commandID] == "healthPickup" && target->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(target);
		if (player != nullptr)
		{
			player->setHealth(player->getHealth() + 5);
		}
	}
	else if (commands[commandID] == "healthPickup" && target->type() == "Model")
	{
		Model* heartModel = dynamic_cast<Model*>(target);
		if (heartModel != nullptr)
		{
			glm::mat4 test = heartModel->getModelMatrix();
			test[3].z = 1000000;
			heartModel->setModelMatrix(test);
		}
		std::vector<glm::vec2> corners;
		corners.push_back(glm::vec2(-101, -101));
		corners.push_back(glm::vec2(-101, -102));
		corners.push_back(glm::vec2(-100, -101));
		corners.push_back(glm::vec2(-100, -102));
		this->setPos(corners);
	}
	else if (commands[commandID] == "phase1" && target->type() == "Enemy")
	{
		EnemyBoss* enemyBoss = dynamic_cast<EnemyBoss*>(target);
		Enemy* enemy = dynamic_cast<Enemy*>(target);

		enemy->setBossImmunity(false);
		enemy->applyDamage(30);
		enemy->setBossImmunity(true);
		if (enemy->getHealth() == 70)
		{
			enemyBoss->setPhase(2);
			enemyBoss->loseTrackOfPlayer(true);

			enemyBoss->setCreateTrigger(true);
		}
		enemyBoss->setRotateNow();
		enemyBoss->setAttacking(false);
		enemyBoss->setChargeCounter(0);
	}
	else if (commands[commandID] == "phase2" && target->type() == "Enemy")
	{
		EnemyBoss* enemyBoss = dynamic_cast<EnemyBoss*>(target);
		Enemy* enemy = dynamic_cast<Enemy*>(target);

		enemy->setBossImmunity(false);
		enemy->applyDamage(30);
		enemy->setBossImmunity(true);
		if (enemy->getHealth() == 40)
		{
			enemyBoss->setPhase(3);
			enemyBoss->setCreateTrigger(true);
		}

		enemyBoss->loseTrackOfPlayer(true);
	}
	else if (commands[commandID] == "finishingBlow" && target->type() == "Enemy")
	{
		EnemyBoss* enemyBoss = dynamic_cast<EnemyBoss*>(target);
		enemyBoss->setChandelierMove();
	}
	else if (commands[commandID] == "playerUnderBoss" && target->type() == "Enemy")
	{
		EnemyBoss* enemyBoss = dynamic_cast<EnemyBoss*>(target);
		if (enemyBoss->getPhase() == 2)
		{
			enemyBoss->setPlayerInWater(!enemyBoss->getPlayerInWater());
			enemyBoss->loseTrackOfPlayer(false);
		}
		if (enemyBoss->getPhase() == 3)
		{
			enemyBoss->loseTrackOfPlayer(!enemyBoss->getPlayerTracked());
		}
	}
	else if (commands[commandID] == "playerUnderBoss" && target->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(target);
		player->setDiving(!player->getDiving());
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
	this->objectsInside = std::vector<bool>(activators.size());
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
	this->objectsInside = std::vector<bool>(activators.size());
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
	this->objectsInside = std::vector<bool>(activators.size());
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
	this->objectsInside = std::vector<bool>(activators.size());
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
	this->objectsInside = std::vector<bool>(activators.size());
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
	this->objectsInside = std::vector<bool>(activators.size());
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
	this->objectsInside = std::vector<bool>(activators.size());
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
	this->objectsInside = std::vector<bool>(activators.size());
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
	this->objectsInside = std::vector<bool>(activators.size());
}
//Destructors
Trigger::~Trigger()
{

}