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
	SoundSystem * sound;
	sf::Clock soundTimer;

	//workarounds for collision
	bool collides;
	bool returnToStart;
	bool collidingWithGround;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemySkeleton(int health, Model* enemyModel, int damage, int immunityTime, bool patrol, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles, SoundSystem * sound);
	virtual ~EnemySkeleton();
	virtual void attackPlayer(float dt, glm::vec3 playerPos);
	virtual void update(float dt, std::vector<Enemy*> &allSmallBats, std::vector<Model*> &allModels, Player* player);
};
