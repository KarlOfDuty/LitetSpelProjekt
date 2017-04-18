#include "PlayerChar.h"


class PlayerButterfly : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
public:
	PlayerButterfly(int HP, Model model);
	virtual ~PlayerButterfly();
	void operator=(const PlayerButterfly &originalObject);
	int getMaxJumps();
	float getJumpHeight();
	void teleport();
};
