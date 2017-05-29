#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H
#include<vector>
#include "Level.h"
class LevelManager
{
private:
	std::vector<Level*> levels;
	int levelIndex;
	int menuIndex;
	EnemyManager *  enemyList;
public:
	Level *currentMenu;
	Level *currentLevel;
	LevelManager(EnemyManager * enemy);
	~LevelManager();
	void startLevel(int level);
	void nextLevel();
	void backToMenu();
};
#endif