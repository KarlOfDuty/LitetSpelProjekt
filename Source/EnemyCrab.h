#include "EnemyChar.h"

class EnemyCrab : public EnemyChar
{
private:
	glm::vec3 originPoint;
	float acceleration;
	bool checkPointReached;
	bool attackNow;
	bool movingLeft;
	bool movingRight;
	sf::Clock walkTimer;
public:
	EnemyCrab(int HP, Model enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyCrab();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos);
};
