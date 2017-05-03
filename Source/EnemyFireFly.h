#include "EnemyChar.h"

class EnemyFireFly : public Enemy
{
private:
	glm::vec3 newCheckpoint;
	int attackRange;
	bool goingRight;
	bool goingLeft;
	bool findPlayer;
	bool checkPointReached;
	sf::Clock waitInAir;
public:
	EnemyFireFly(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyFireFly();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};
