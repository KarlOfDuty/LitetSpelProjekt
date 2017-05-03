#include "EnemyChar.h"

class EnemyCrab : public Enemy
{
private:
	glm::vec3 originPoint;
	float acceleration;
	bool checkPointReached;
	bool attacking;
	bool movingLeft;
	bool movingRight;
	sf::Clock walkTimer;
public:
	EnemyCrab(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyCrab();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};
