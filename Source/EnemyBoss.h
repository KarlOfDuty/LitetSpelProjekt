#include "Enemy.h"

class EnemyBoss : public Enemy
{
private:
	bool fightStart;
	bool phase1;
	bool phase2;
	bool phase3;
	bool rotateLeft;
	int chargeCounter;
	sf::Clock dazeTimer;
	sf::Clock walkTimer;
	glm::vec3 originPoint;
	float acceleration;
	bool attacking;
	bool movingLeft;
	bool movingRight;
	bool checkPointReached;
	std::vector<Trigger*> weakPointsArr;
	Model* boxModel;
	std::vector<Model*> debugWeakPointsBox;

	//workarounds for collision
	bool collides;
	bool collidingWithGround;
	bool returnToStart;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemyBoss(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos, glm::vec3 scaleFactor);
	virtual ~EnemyBoss();
	void weakPoints(std::vector<GameObject*> allProjectiles);
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player);
	std::vector<Model*> getDebugModels();
};
