#include "PlayerChar.h"

class PlayerShark : public PlayerChar
{
private:

public:
	PlayerShark(int HP, Model model);
	virtual ~PlayerShark();
	void operator=(const PlayerShark &originalObject);
	void dive();
};
