#include "EnemyChar.h"

class EnemyBoss : public Enemy
{
private:
	bool phase1;
	bool phase2;
	bool phase3;
	int chargeCounter;
	sf::Clock dazeTimer;
	sf::Clock walkTimer;
	glm::vec3 originPoint;
	float acceleration;
	bool attacking;
	bool movingLeft;
	bool movingRight;
	bool checkPointReached;
public:
	EnemyBoss(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBoss();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};
