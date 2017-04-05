#include "PlayerChar.h"


class PlayerBird : public PlayerChar
{
private:

public:
	PlayerBird(int HP);
	~PlayerBird();
	void operator=(const PlayerBird &originalObject);
	bool jumpAllowed();
	virtual void jump();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void attack();

};
