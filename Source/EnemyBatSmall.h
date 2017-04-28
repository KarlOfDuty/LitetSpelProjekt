#include "EnemyChar.h"

class EnemyBatSmall : public EnemyChar
{
private:
	bool goForPlayer;
	bool findPlayer;
	glm::vec3 newCheckpoint;
	sf::Clock checkPointClock;
public:
	EnemyBatSmall(int HP, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBatSmall();
	float RandomNumber(float Min, float Max);
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos);
};