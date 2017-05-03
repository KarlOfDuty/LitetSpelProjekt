#include "PlayerChar.h"


class PlayerButterfly : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
public:
	PlayerButterfly(int health, Model model, bool inWater);
	virtual ~PlayerButterfly();
	void operator=(const PlayerButterfly &originalObject);
	int getMaxJumps();
	float getJumpHeight();
	void waterEffect();
	void teleport();

	void lightAttack(std::vector<Projectile*> &allProjectiles, glm::vec2 position, glm::vec2 direction)const;
	void heavyAttack()const;
};
