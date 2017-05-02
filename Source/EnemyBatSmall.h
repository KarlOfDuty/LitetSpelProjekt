#include "EnemyChar.h"

class EnemyBatSmall : public EnemyChar
{
private:
	bool goForPlayer;
	bool findPlayer;
	bool returnToStart;
	bool collision;
	glm::vec3 newCheckpoint;
	glm::vec3 startPosition;
	sf::Clock collisionClock;
	std::random_device rd;
public:
	EnemyBatSmall(int HP, Model* enemyModel, int damage, glm::vec3 enemyStartPos);
	virtual ~EnemyBatSmall();
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<EnemyChar*> smallBatsPos, std::vector<Model*> &allModels);
};
