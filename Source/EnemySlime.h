#include "EnemyChar.h"

class EnemySlime : public Enemy
{
private:
	bool checkPointReached;

	//workarounds for collision
	bool collides;
	bool returnToStart;
	bool collidingWithGround;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemySlime(int health, Model* model, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemySlime();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};

