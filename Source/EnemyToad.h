#include "EnemyChar.h"

class EnemyToad : public EnemyChar
{
private:
	bool checkPointReached;
public:
	EnemyToad(int HP, Model enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyToad();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint);
};

