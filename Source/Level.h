#ifndef LEVEL_H
#define LEVEL_H
#include "Model.h"
#include "Trigger.h"
#include <SFML/Window.hpp>
#include "SoundSystem.h"
class Level
{
private:
	glm::vec3 playerPos = glm::vec3(0,2,0);
	std::vector<Model*> modelLibrary;
	std::vector<Model*> staticModels;
	std::vector<Model*> collidables;
	std::vector<Model*> dynamicModels;
	std::vector<Trigger*> triggerBoxes;
	std::vector<std::string> modelFilePaths;
public:
	void loadModels();
	void setupModels();
	void setupMenuModels();
	void unloadModels();
	void setupTriggers(Player* player);
	void updateTriggers(float dt);
	void spinMenu(float dt);
	void deleteTriggers();
	void playMusic(SoundSystem *soundSystem);
	void stopMusic(SoundSystem *soundSystem);
	std::vector<Model*> getDynamicModels();
	std::vector<Model*>& getStaticModels();
	std::vector<Trigger*> getTriggers();
	glm::vec3 getPlayerPos();
	Level();
	Level(std::string filepath);
	~Level();
};
#endif