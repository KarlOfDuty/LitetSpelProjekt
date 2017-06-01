#include "Enemy.h"

class EnemyBatSmall : public Enemy
{
private:
	bool goForPlayer;
	glm::vec3 checkpoint;
	std::random_device rd;

	//workarounds for collision
	bool collides;
	int collisionCounter;
	bool collidingWithGround;
	glm::vec3 startPosition;
	bool returnToStart;
	sf::Clock timeSinceCollision;
	sf::Clock collisionTime;
	SoundSystem * sound;
	sf::Clock soundTimer;
public:
	EnemyBatSmall(int health, Model* enemyModel, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, SoundSystem * sound);
	virtual ~EnemyBatSmall();
	virtual void attackPlayer(float dt, glm::vec3 playerPos);
	virtual void update(float dt, std::vector<Enemy*> &allSmallBats, std::vector<Model*> &allModels, Player* player);
};
