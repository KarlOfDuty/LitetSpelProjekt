#include "PlayerChar.h"

class PlayerShark : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
public:
	PlayerShark(int health, Model model, bool inWater);
	virtual ~PlayerShark();
	void operator=(const PlayerShark &originalObject);
	int getMaxJumps();
	float getJumpHeight();
	void waterEffect();
	void dive();

	void lightAttack(std::vector<Projectile*> &allProjectiles, glm::vec2 position, glm::vec2 direction)const;
	void heavyAttack()const;
};
