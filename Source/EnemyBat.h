#include "EnemyChar.h"

class EnemyBat : public EnemyChar
{
private:
	float yValue;
	float midX;
	glm::vec3 newCheckpoint;
	bool goingRight;
	bool goingLeft;
	bool findPlayer;
	bool clockRestart;
	bool checkPointReached;
	sf::Clock waitInAir;
public:
	EnemyBat(int HP, Model enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBat();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos);
};
