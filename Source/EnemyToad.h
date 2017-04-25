#include "EnemyChar.h"

class EnemyToad : public EnemyChar
{
private:
	bool checkPointReached;
	sf::Clock jumpTimer;
	bool movingLeft;
	bool movingRight;
public:
	EnemyToad(int HP, Model enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyToad();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos);
};

