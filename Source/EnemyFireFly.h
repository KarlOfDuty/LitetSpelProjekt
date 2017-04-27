#include "EnemyChar.h"

class EnemyFireFly : public EnemyChar
{
private:
	glm::vec3 newCheckpoint;
	int attackRange;
	bool goingRight;
	bool goingLeft;
	bool findPlayer;
	bool clockRestart;
	bool checkPointReached;
	sf::Clock waitInAir;
public:
	EnemyFireFly(int HP, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyFireFly();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels);
};
