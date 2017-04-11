#include "PlayerChar.h"

class PlayerShark : public PlayerChar
{
private:
	int maxJumps;
public:
	PlayerShark(int HP, Model model);
	virtual ~PlayerShark();
	void operator=(const PlayerShark &originalObject);
	int getMaxJumps();
	void dive();
};
