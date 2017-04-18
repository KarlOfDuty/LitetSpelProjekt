#include "EnemyChar.h"

class EnemySlime : public EnemyChar
{
private:
	bool checkPointReached;
public:
	EnemySlime(int HP, Model enemyModel, int damage, glm::vec3 enemyStartPos, glm::vec3 checkPoint);
	virtual ~EnemySlime();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos);
};

