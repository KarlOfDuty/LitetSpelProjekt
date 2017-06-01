#include "Enemy.h"

class EnemySlime : public Enemy
{
private:
	bool checkPointReached;
	bool rotateLeft;

	//workarounds for collision
	bool collides;
	bool returnToStart;
	bool collidingWithGround;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
	SoundSystem * sound;
	sf::Clock soundTimer;

public:
	EnemySlime(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, SoundSystem * sound);
	virtual ~EnemySlime();
	virtual void attackPlayer(float dt, glm::vec3 playerPos);
	virtual void update(float dt, std::vector<Enemy*> &allSmallBats, std::vector<Model*> &allModels, Player* player);
};

