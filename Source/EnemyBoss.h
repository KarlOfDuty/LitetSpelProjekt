#include "EnemyChar.h"

class EnemyBoss : public EnemyChar
{
private:
	bool phase1, phase2, phase3;
	int chargeCounter;
	sf::Clock dazeTimer;
	sf::Clock walkTimer;
	glm::vec3 originPoint;
	float acceleration;
	bool attackNow;
	bool movingLeft;
	bool movingRight;
	bool checkPointReached;
public:
	EnemyBoss(int HP, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBoss();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels);
};
