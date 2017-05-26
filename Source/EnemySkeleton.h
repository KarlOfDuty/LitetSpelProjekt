#include "Enemy.h"

class EnemySkeleton : public Enemy
{
private:
	bool checkPointReached;
	sf::Clock attackCooldown;
	sf::Clock waitTimer;
	sf::Clock waitBeforeAttack;
	sf::Clock jumpDelay;
	float acceleration;
	bool patrol;
	bool rotateLeft;
	float attackRange;
	std::vector<Projectile*> *allProjectiles;
	Model* box;
	bool attack;
	bool dodgeLeft;
	glm::vec3 Dodgecheckpoint;
	bool checkPointGiven;
	bool jumped;

	//workarounds for collision
	bool collides;
	bool returnToStart;
	bool collidingWithGround;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemySkeleton(int health, Model* enemyModel, int damage, int immunityTime, bool patrol, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles, SoundSystem * sound);
	virtual ~EnemySkeleton();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player);
};
