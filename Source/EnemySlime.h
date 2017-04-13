#include "EnemyChar.h"

class EnemySlime : public EnemyChar
{
private:

public:
	EnemySlime(int HP, Model enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemySlime();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos);
};

