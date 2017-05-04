#include "EnemyChar.h"

class EnemySkeleton : public Enemy
{
private:
	bool checkPointReached;
	bool patrol;

	//workarounds for collision
	bool collides;
	bool returnToStart;
	bool collidingWithGround;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemySkeleton(int health, Model* enemyModel, int damage, bool patrol, glm::vec3 enemyStartPos);
	virtual ~EnemySkeleton();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};
