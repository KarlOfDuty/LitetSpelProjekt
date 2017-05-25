#include "LevelManager.h"
LevelManager::LevelManager()
{
	levels.push_back(new Level());
	levels.push_back(new Level());
	menuIndex = 0;
	levelIndex = 1;
	currentMenu = levels[menuIndex];
	currentLevel = levels[levelIndex];
}

LevelManager::~LevelManager()
{

}
//Unload current level before switching
void LevelManager::startLevel(int level)
{
	levelIndex = level;
	currentLevel = levels[levelIndex];
}
void LevelManager::nextLevel()
{
	if (levels.size() > levelIndex + 1)
	{
		levelIndex++;
		currentLevel = levels[levelIndex];
	}
	else
	{
		backToMenu();
	}
}
void LevelManager::backToMenu()
{
	levelIndex = 0;
	currentLevel = levels[levelIndex];
}