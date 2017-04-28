#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H
#include<vector>
#include "Level.h"
class LevelManager
{
private:
	std::vector<Level*> levels;
	int levelIndex;
public:
	Level *currentLevel;
	LevelManager();
	~LevelManager();
	void startLevel(int level);
	void nextLevel();
	void backToMenu();
};
#endif