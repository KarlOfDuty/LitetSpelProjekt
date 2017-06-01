#include "PlayerChar.h"


class PlayerBird : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
	Model* box;
	sf::Clock attackCooldown;
public:
	PlayerBird(Model* model, bool inWater);
	virtual ~PlayerBird();
	void operator=(const PlayerBird &originalObject);
	float getJumpHeight();
	int getMaxJumps();
	void meleeAttack(std::vector<Projectile*> &allAttackBoxes, glm::vec2 position, glm::vec2 direction, float velocity);
};
