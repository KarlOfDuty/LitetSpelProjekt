#include "LevelManager.h"
LevelManager::LevelManager(EnemyManager * enemy)
{
	this->enemyList = enemy;
	levels.push_back(new Level("config/level1.ini", enemyList));
	levels.push_back(new Level("config/level2.ini", enemyList));
	levelIndex = 0;
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
	levelIndex++;
	if (levelIndex < levels.size())
	{
		currentLevel = levels[levelIndex];
	}
	else
	{
		backToMenu();
		levelIndex = 0;
		currentLevel = levels[0];
	}
}
void LevelManager::backToMenu()
{
	levelIndex = 0;
	currentLevel = levels[levelIndex];
}