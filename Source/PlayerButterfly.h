#include "PlayerChar.h"


class PlayerButterfly : public PlayerChar
{
private:
	Model butterflyModel;
public:
	PlayerButterfly(int HP);
	virtual ~PlayerButterfly();
	PlayerButterfly(const PlayerButterfly & originalObject);
	void operator=(const PlayerButterfly &originalObject);
	void teleport();
	virtual Model getModel()const;
	virtual void jump();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void attack();

};
