#include "Enemy.h"

class EnemyBat : public Enemy
{
private:
	float midX;
	glm::vec3 newCheckPoint;
	glm::vec3 oldCheckPoint;
	bool goingRight;
	bool goingLeft;
	bool goUp;
	bool swoopAttack;
	sf::Clock waitInAir;
	bool checkPointReached;
	bool xPointReached;
	bool yPointReached;

	//workarounds for collision
	bool collides;
	bool collidingWithGround;
	bool returnToStart;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemyBat(int health, Model* enemyModel, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, SoundSystem * sound);
	virtual ~EnemyBat();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player);
};
