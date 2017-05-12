#include "Enemy.h"

class EnemyToad : public Enemy
{
private:
	int attackRange;
	bool checkPointReached;
	sf::Clock jumpTimer;
	bool movingLeft;
	bool movingRight;
	bool rotateLeft;
	sf::Clock attackCooldown;
	std::vector<Projectile*> *allProjectiles;
	Model* projectileModel;

	//workarounds for collision
	bool collides;
	bool collidingWithGround;
	int collisionCounterToad;
	bool returnToStart;
	sf::Clock timeSinceCollision;
	glm::vec3 startPosition;
public:
	EnemyToad(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles);
	virtual ~EnemyToad();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player);
};

