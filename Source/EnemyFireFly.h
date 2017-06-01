#include "Enemy.h"

class EnemyFireFly : public Enemy
{
private:
	int attackRange;
	sf::Clock attackCooldown;
	std::vector<Projectile*> *allProjectiles;
	Model* projectileModel;
	//workarounds for collision
	bool collides;
	bool collidingWithGround;
	bool returnToStart;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
	SoundSystem * sound;
	sf::Clock soundTimer;

	sf::Clock timeSinceCollision;

public:
	EnemyFireFly(int health, Model* enemyModel, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles, SoundSystem * sound);
	virtual ~EnemyFireFly();
	virtual void attackPlayer(float dt, glm::vec3 playerPos);
	virtual void update(float dt, std::vector<Enemy*> &allSmallBats, std::vector<Model*> &allModels, Player* player);
};
