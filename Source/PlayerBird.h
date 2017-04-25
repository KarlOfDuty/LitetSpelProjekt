#include "PlayerChar.h"


class PlayerBird : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
public:
	PlayerBird(int HP, Model model, bool inWater);
	virtual ~PlayerBird();
	void operator=(const PlayerBird &originalObject);
	float getJumpHeight();
	int getMaxJumps();
	void waterEffect();
};
