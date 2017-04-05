#include "PlayerChar.h"

class PlayerShark : public PlayerChar
{
private:

public:
	PlayerShark(int HP);
	~PlayerShark();
	void operator=(const PlayerShark &originalObject);
	void dive();
	virtual void jump();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void attack();

};
