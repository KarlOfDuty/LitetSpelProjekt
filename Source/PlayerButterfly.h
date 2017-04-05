#include "PlayerChar.h"


class PlayerButterfly : public PlayerChar
{
private:
	
public:
	PlayerButterfly(int HP);
	~PlayerButterfly();
	void operator=(const PlayerButterfly &originalObject);
	void teleport();
	virtual void jump();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void attack();

};
