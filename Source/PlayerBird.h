#include "PlayerChar.h"


class PlayerBird : public PlayerChar
{
private:
	bool jumpAllowed;
public:
	PlayerBird(int HP, Model model);
	virtual ~PlayerBird();
	void setDoubleJump(bool doubleJumpAllowed);
	bool getDoubleJump();
};
