#include "EnemyChar.h"

class EnemyBatSmall : public Enemy
{
private:
	//TODO: Make sense
	bool goForPlayer;
	bool findPlayer;
	bool returnToStart;
	bool collides;
	glm::vec3 checkpoint;
	glm::vec3 startPosition;
	sf::Clock collisionTime;
	std::random_device rd;
public:
	EnemyBatSmall(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBatSmall();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};
