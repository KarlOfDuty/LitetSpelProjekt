#include "PlayerChar.h"

class PlayerShark : public PlayerChar
{
private:
	int maxJumps;
	float jumpHeight;
public:
	PlayerShark(int HP, Model model);
	virtual ~PlayerShark();
	void operator=(const PlayerShark &originalObject);
	int getMaxJumps();
	float getJumpHeight();
	void dive();
};
