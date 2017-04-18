#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H
#include<vector>
#include "Level.h"
class LevelManager
{
private:
	std::vector<Level*> levels;

public:
	Level *currentLevel;
	LevelManager();
	~LevelManager();
};
#endif