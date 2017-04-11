#include "PlayerChar.h"


class PlayerButterfly : public PlayerChar
{
private:

public:
	PlayerButterfly(int HP, Model model);
	virtual ~PlayerButterfly();
	void teleport();
};
