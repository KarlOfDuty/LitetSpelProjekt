#include "LevelManager.h"
LevelManager::LevelManager()
{
	this->enemyList = nullptr;
	levels.push_back(new Level("config/level0.ini", enemyList));
	levels.push_back(new Level("config/level1.ini", enemyList));
	levels.push_back(new Level("config/level2.ini", enemyList));
	levelIndex = 1;
	currentLevel = levels[levelIndex];
	currentMenu = levels[0];
}
LevelManager::LevelManager(EnemyManager * enemy)
{
	this->enemyList = enemy;
	levels.push_back(new Level("config/level0.ini", enemyList));
	levels.push_back(new Level("config/level1.ini", enemyList));
	levels.push_back(new Level("config/level2.ini", enemyList));
	levels.push_back(new Level("config/level4.ini", enemyList));
	levelIndex = 1;
	currentLevel = levels[levelIndex];
	currentMenu = levels[0];
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