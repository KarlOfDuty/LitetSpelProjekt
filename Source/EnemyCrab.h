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

	//workarounds for collision
	bool collides;
	bool collidingWithGround;
	bool returnToStart;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemyCrab(int health, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyCrab();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, std::vector<glm::vec2> playerPoints);
};
