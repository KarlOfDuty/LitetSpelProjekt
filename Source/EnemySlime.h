#include "EnemyChar.h"

class EnemySlime : public EnemyChar
{
private:

public:
	EnemySlime(int HP, Model enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemySlime();
};

