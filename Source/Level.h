#ifndef LEVEL_H
#define LEVEL_H
#include "Model.h"
#include "Trigger.h"
#include <SFML/Window.hpp>
class Level
{
private:
	glm::vec3 playerPos = glm::vec3(0,2,0);
	std::vector<Model*> modelLibrary;
	std::vector<Model*> staticModels;
	std::vector<Model*> dynamicModels;
	std::vector<Trigger*> triggerBoxes;
	std::vector<std::string> modelFilePaths;
public:
	void loadModels();
	void setupModels();
	void unloadModels();
	void setupTriggers(Player* player);
	void deleteTriggers();
	std::vector<Model*> getStaticModels();
	std::vector<Trigger*> getTriggers();
	glm::vec3 getPlayerPos();
	Level();
	Level(std::string filepath);
	~Level();
};
#endif