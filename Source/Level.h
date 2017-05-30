#ifndef LEVEL_H
#define LEVEL_H
#include "Model.h"
#include "Trigger.h"
#include <SFML/Window.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include "SoundSystem.h"
//Turns on console feedback for reading of model files
class Level
{
private:

	glm::vec3 playerPos = glm::vec3(0, 2, 0);
	std::vector<Model*> staticModels;
	std::vector<Model*> colliders;
	std::vector<Model*> dynamicModels;
	std::vector<Trigger*> triggerBoxes;
public:
	std::string filePath;
	void loadModels();
	void loadLevel();
	bool readModels(const char* filePath, std::vector<Model*> &modelVector);
	void unloadModels();
	void setupTriggers(Player* player);
	void updateTriggers(float dt);
	void deleteTriggers();
	void playMusic(SoundSystem *soundSystem);
	void stopMusic(SoundSystem *soundSystem);
	std::vector<Model*> getDynamicModels();
	std::vector<Model*>& getStaticModels();
	std::vector<Model*>& getCollisionBoxes();
	std::vector<Trigger*> getTriggers();
	glm::vec3 getPlayerPos();
	void createPickup(Model * pickupModel, glm::vec2 position, std::string triggerName, Player * player);
	Level();
	Level(std::string filePath);
	~Level();
};
#endif