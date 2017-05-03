#include "EnemyChar.h"

class EnemyBat : public Enemy
{
private:
	float midX;
	glm::vec3 checkpoint;
	bool goingRight;
	bool goingLeft;
	bool swoopAttack;
	sf::Clock waitInAir;
	bool clockRestart;
	bool checkPointReached;

	//workarounds for collision
	bool collides;
	bool returnToStart;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemyBat(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBat();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels);
};
