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
		int objectsFound = 0;
		for (int i = 0; i < activators.size(); i++)
		{
			if (collision::collision(corners, activators[i]->getPoints()))
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
		if (timer > settings.frequency)
		{

			//onEnter
			if (settings.onEnter && objectsFound > objectsInside)
			{
				for (int i = 0; i < objectsFound - objectsInside; i++)
				{
					activate(dt);
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
						activate(dt);
					}
				}
				else
				{
					activate(dt);
				}
				timer = 0;
				triggered = true;
			}
			//onExit
			if (settings.onExit && objectsFound < objectsInside)
			{
				for (int i = 0; i < objectsInside - objectsFound; i++)
				{
					activate(dt);
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
						activate(dt);
					}
				}
				else
				{
					activate(dt);
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
						activate(dt);
					}
				}
				else
				{
					activate(dt);
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
						activate(dt);
					}
				}
				else
				{
					activate(dt);
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
	}

	return triggered;
}
//Activates the trigger
void Trigger::activate(float dt)
{
	if (settings.accociativeActions)
	{
		//Activate each command on their counterpart in targets
		for (int i = 0; i < commands.size() && i < targets.size(); i++)
		{
			runCommand(i,i, dt);
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
				runCommand(i,j, dt);
				//numberOfActivationsAllowed
				if (settings.numberOfActivationsAllowed != -1)
				{
					settings.numberOfActivationsAllowed--;
				}
			}
		}
	}
}
void Trigger::runCommand(int commandID, int targetID, float dt)
{
	//Add new commands here
	if (commands[commandID] == "hellogais")
	{
		std::cout << "Hello gais" << std::endl;
	}
	else if (commands[commandID] == "water" && targets[targetID]->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(targets[targetID]);
		player->setDiving(!player->getDiving());
	}
	else if (commands[commandID] == "water" && targets[targetID]->type() == "Enemy")
	{
		Enemy* enemy = dynamic_cast<Enemy*>(targets[targetID]);
		enemy->setHealth(0);
	}
	else if (commands[commandID] == "nextLevel")
	{
		nextLevel = true;
	}
	else if (commands[commandID] == "fire" && targets[targetID]->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(targets[targetID]);
		player->applyDamage(1);
	}
	else if (commands[commandID] == "spikes" && targets[targetID]->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(targets[targetID]);
		player->applyDamage(10);
	}
	else if (commands[commandID] == "kill" && targets[targetID]->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(targets[targetID]);
		player->setHealth(0);
	}
	else if (commands[commandID] == "kill" && targets[targetID]->type() == "Enemy")
	{
		Enemy* enemy = dynamic_cast<Enemy*>(targets[targetID]);
		enemy->setHealth(0);
	}
	else if (commands[commandID] == "healthPickup" && targets[targetID]->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(targets[targetID]);
		if (player != nullptr)
		{
			player->setHealth(player->getHealth() + 5);
		}
	}
	else if (commands[commandID] == "healthPickup" && targets[targetID]->type() == "Model")
	{
		Model* heartModel = dynamic_cast<Model*>(targets[targetID]);
		if (heartModel != nullptr)
		{
			glm::mat4 test = heartModel->getModelMatrix();
			test[3].z = 1000000;
			heartModel->setModelMatrix(test);
		}
		delete this;
	}
	else if (commands[commandID] == "phase1" && targets[targetID]->type() == "Enemy")
	{
		EnemyBoss* enemyBoss = dynamic_cast<EnemyBoss*>(targets[targetID]);
		Enemy* enemy = dynamic_cast<Enemy*>(targets[targetID]);

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
	else if (commands[commandID] == "phase2" && targets[targetID]->type() == "Enemy")
	{
		EnemyBoss* enemyBoss = dynamic_cast<EnemyBoss*>(targets[targetID]);
		Enemy* enemy = dynamic_cast<Enemy*>(targets[targetID]);

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
	else if (commands[commandID] == "finishingBlow" && targets[targetID]->type() == "Enemy")
	{
		EnemyBoss* enemyBoss = dynamic_cast<EnemyBoss*>(targets[targetID]);
		enemyBoss->setChandelierMove();
	}
	else if (commands[commandID] == "playerUnderBoss" && targets[targetID]->type() == "Enemy")
	{
		EnemyBoss* enemyBoss = dynamic_cast<EnemyBoss*>(targets[targetID]);
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
	else if (commands[commandID] == "playerUnderBoss" && targets[targetID]->type() == "Player")
	{
		Player* player = dynamic_cast<Player*>(targets[targetID]);
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