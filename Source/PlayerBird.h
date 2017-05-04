#include "PlayerChar.h"


class PlayerBird : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
	Model* arrowModel;
	bool lightAttackBool;
	bool heavyAttackBool;
public:
	float arrowVelocity;
	PlayerBird(int health, Model model, bool inWater);
	virtual ~PlayerBird();
	void operator=(const PlayerBird &originalObject);
	float getJumpHeight();
	int getMaxJumps();
	void waterEffect();
	void shootArrow(std::vector<Projectile*> &allProjectiles, glm::vec2 position, glm::vec2 direction)const;
};
