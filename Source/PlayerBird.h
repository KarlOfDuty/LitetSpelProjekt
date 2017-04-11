#include "PlayerChar.h"


class PlayerBird : public PlayerChar
{
private:
	int maxJumps;
public:
	PlayerBird(int HP, Model model);
	virtual ~PlayerBird();
	void operator=(const PlayerBird &originalObject);
	int getMaxJumps();
};
