#include "PlayerChar.h"


class PlayerButterfly : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
public:
	PlayerButterfly(int HP, Model model, bool inWater);
	virtual ~PlayerButterfly();
	void operator=(const PlayerButterfly &originalObject);
	int getMaxJumps();
	float getJumpHeight();
	void waterEffect();
	void teleport();
};
