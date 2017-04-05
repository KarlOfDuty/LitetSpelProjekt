#include "PlayerChar.h"

class PlayerShark : public PlayerChar
{
private:
	Model sharkModel;
public:
	PlayerShark(int HP);
	virtual ~PlayerShark();
	PlayerShark(const PlayerShark & originalObject);
	void operator=(const PlayerShark &originalObject);
	void dive();
	virtual Model getModel()const;
	virtual void jump();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void attack();

};
