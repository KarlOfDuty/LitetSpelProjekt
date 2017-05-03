#include "EnemyChar.h"

class EnemyBatSmall : public Enemy
{
private:
	bool goForPlayer;
	glm::vec3 checkpoint;
	sf::Clock timeSinceCollision;
	std::random_device rd;

	//workarounds for collision
	bool collides;
	int collisionCounter;
	glm::vec3 startPosition;
	bool returnToStart;
	sf::Clock collisionTime;
public:
	EnemyBatSmall(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBatSmall();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};
