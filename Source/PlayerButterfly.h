#include "PlayerChar.h"


class PlayerButterfly : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
	Model* box;
public:
	PlayerButterfly(Model model, bool inWater);
	virtual ~PlayerButterfly();
	void operator=(const PlayerButterfly &originalObject);
	int getMaxJumps();
	float getJumpHeight();
	void teleport();
	void shootAoe(std::vector<Model*> &allStaticModels, std::vector<Projectile*> &allProjectiles, glm::vec2 position);
};
