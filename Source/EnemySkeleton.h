#include "EnemyChar.h"

class EnemySkeleton : public EnemyChar
{
private:
	bool checkPointReached;
	bool patrol;
public:
	EnemySkeleton(int HP, Model enemyModel, int damage, bool patrol, glm::vec3 enemyStartPos);
	virtual ~EnemySkeleton();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos);
};
