#include "PlayerChar.h"


class PlayerBird : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
public:
	PlayerBird(int HP, Model model);
	virtual ~PlayerBird();
	void operator=(const PlayerBird &originalObject);
	float getJumpHeight();
	int getMaxJumps();
};
