#include "EnemyChar.h"

class EnemyBat : public Enemy
{
private:
	float midX;
	glm::vec3 checkpoint;
	bool goingRight;
	bool goingLeft;
	bool findPlayer;
	bool clockRestart;
	bool checkPointReached;
	sf::Clock waitInAir;
public:
	EnemyBat(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBat();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};
