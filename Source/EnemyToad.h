#include "EnemyChar.h"

class EnemyToad : public Enemy
{
private:
	bool checkPointReached;
	sf::Clock jumpTimer;
	bool movingLeft;
	bool movingRight;

	//workarounds for collision
	bool collides;
	bool collidingWithGround;
	bool returnToStart;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemyToad(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyToad();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};

