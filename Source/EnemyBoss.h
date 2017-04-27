#include "EnemyChar.h"

class EnemyBoss : public EnemyChar
{
private:
	glm::vec3 newCheckpoint;
	bool goingRight;
	bool goingLeft;
	bool findPlayer;
	bool clockRestart;
	bool checkPointReached;
	sf::Clock waitInAir;
public:
	EnemyBoss(int HP, Model enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBoss();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPos);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPos, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos);
};
