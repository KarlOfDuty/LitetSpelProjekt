#include "Enemy.h"

class EnemyCrab : public Enemy
{
private:
	glm::vec3 oldOriginPoint;
	float acceleration;
	bool checkPointReached;
	bool moving;
	bool movingLeft;
	bool movingRight;
	sf::Clock walkTimer;
	SoundSystem * sound;
	sf::Clock soundTimer;

	//workarounds for collision
	bool collides;
	bool collidingWithGround;
	bool returnToStart;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
	int collisionCounter;
	sf::Clock timeSinceCollision;
public:
	EnemyCrab(int health, Model* enemyModel, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, SoundSystem * sound);
	virtual ~EnemyCrab();
	virtual void attackPlayer(float dt, glm::vec3 playerPos);
	virtual void update(float dt, std::vector<Enemy*> &allSmallBats, std::vector<Model*> &allModels, Player* player);
};
