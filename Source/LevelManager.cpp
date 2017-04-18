#include "LevelManager.h"
LevelManager::LevelManager()
{
	levels.push_back(new Level());
	currentLevel = levels[0];
}

LevelManager::~LevelManager()
{

}