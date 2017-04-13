#include "LevelManager.h"
LevelManager::LevelManager()
{
	levels.push_back(new Level());
	levelIndex = 0;
	currentLevel = levels[levelIndex];
}

LevelManager::~LevelManager()
{

}
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