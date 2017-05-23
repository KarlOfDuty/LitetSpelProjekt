#include "Enemy.h"

class EnemyBoss : public Enemy
{
private:
	bool blockExit;
	int phase;
	//phase 1
	int chargeCounter;
	sf::Clock dazeTimer;
	sf::Clock walkTimer;
	float acceleration;
	bool attacking;
	glm::vec3 originPoint;

	//phase 2
	bool playerTracked;
	glm::vec3 oldPlayerPos;
	bool wallDestroyed;
	glm::vec3 centerOfRoom;
	bool checkPointReached;
	sf::Clock attackCooldown;
	std::vector<Projectile*> *allProjectiles;
	Model* projectileModel;
	bool removeGround;
	sf::Clock removeGroundTimer;
	bool inRightCorner;

	//phase 3

	//rotation stuff
	bool rotateLeft;
	bool movingLeft;
	bool movingRight;
	bool rotateNow;

	//trigger stuff
	bool createTrigger;
	std::vector<Trigger*> waterArea;
	std::vector<glm::vec2> weakPoint;
	std::vector<Trigger*> weakPointsArr;
	Model* boxModel;
	std::vector<Model*> debugWeakPointsBox;
	std::vector<glm::vec4> corners;
	std::vector<GameObject*> playerProjectiles;
	std::vector<GameObject*> playerAndBoss;

	//workarounds for collision
	bool collides;
	bool collidingWithGround;
	bool returnToStart;
	sf::Clock collisionTime;
	glm::vec3 startPosition;
public:
	EnemyBoss(int health, Model* enemyModel, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, std::vector<Projectile*> *allProjectiles);
	virtual ~EnemyBoss();
	void setWaterArea(Player* player, std::vector<Model*> &allModels);
	void weakPoints(std::vector<GameObject*> allProjectiles, std::string command, int amountOfTimes);
	void editWeakPoint(float xValue, float yValue, Player* player);
	void setPhase(int phase);
	void setCreateTrigger(bool createTrigger);
	void setChargeCounter(int amountOfCharges);
	void setRotateToPlayer(Player *player);
	void setRotateToOrigin();
	void setRotateNow();
	void loseTrackOfPlayer(bool playerIsFound);
	virtual void attackPlayer(float dt, glm::vec3 playerPos, glm::vec3 enemyPosCurrent);
	virtual void updateThis(float dt, glm::vec3 enemyPosCurrent, glm::vec3 checkPoint, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player);
	std::vector<Model*> getDebugModels();
};
